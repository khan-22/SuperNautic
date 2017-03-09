#include "GFX/Rendering/ForwardRenderer.hpp"

#include "GFX/Rendering/Renderable3D.hpp"
#include "Core/Utility/Camera.h"

#define NUMOFLIGHTS 8


using namespace GFX;

ForwardRenderer::ForwardRenderer()
	: _window(nullptr)
{
}

ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer)
{
	_resultFramebuffer = resultFramebuffer;

	_shader = ShaderCache::get("geometry_forward");
	_shader.get()->bind();
    _shader.get()->setSampler("uDiffuse", 0);
    _shader.get()->setSampler("uSpecular", 1);
    _shader.get()->setSampler("uNormal", 2);
    _shader.get()->setSampler("uIllumination", 3);

	_window = window;
	_x		= x;
	_y		= y;
	_width	= width;
	_height = height;

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	_actualX = static_cast<GLsizei>(_x * windowWidth);
	_actualY = static_cast<GLsizei>(_y * windowHeight);
	_actualWidth = static_cast<GLsizei>(_width  * windowWidth);
	_actualHeight = static_cast<GLsizei>(_height * windowHeight);
}

void ForwardRenderer::pushPointLight(PointLight& pointLight)
{
	if (pointLight.bGetActive())
	{
		_pointLights.push_back(&pointLight);
	}

	if (_pointLights.size() > 8)
	{
		LOG_ERROR("Cannot draw more than 32 lights in a scene");
	}
}

void ForwardRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}


void ForwardRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

Framebuffer::DEFAULT.bindWrite();

	glViewport(_actualX, _actualY, _actualWidth, _actualHeight);

	glDisable(GL_BLEND);

	_shader.get()->bind();

	_shader.get()->setUniform("uViewPos", camera.getPosition());


	// Send all point light data as a uniform array struct
	int lightCount = std::min((int)_pointLights.size(), NUMOFLIGHTS);

	std::vector<glm::vec3> pointLightPos;
	std::vector<glm::vec3> pointLightColor;
	std::vector<float>	   pointLightIntensity;
	std::vector<glm::vec3> pointLightProperties;

	pointLightPos.reserve(NUMOFLIGHTS);
	pointLightColor.reserve(NUMOFLIGHTS);
	pointLightIntensity.reserve(NUMOFLIGHTS);
	pointLightProperties.reserve(NUMOFLIGHTS);

	for (int i = 0; i < lightCount; i++)
	{
		pointLightPos.push_back(_pointLights[i]->getPosition());
		pointLightColor.push_back(_pointLights[i]->getLightProperties().diffuseColor);
		pointLightIntensity.push_back(_pointLights[i]->getLightProperties().intensity);

		glm::vec3 properties(_pointLights[i]->getLightProperties().constant, _pointLights[i]->getLightProperties().linear, _pointLights[i]->getLightProperties().quadratic);
		pointLightProperties.push_back(properties);

	}
	for (int i = lightCount; i < NUMOFLIGHTS; i++)
	{
		pointLightPos.push_back({ 0.f, 0.f, 0.f });
		pointLightColor.push_back({ 0.f, 0.f, 0.f });
		pointLightIntensity.push_back({ 1.f });
		pointLightProperties.push_back({ 1.f, 0.f, 0.f });
	}

	_shader.get()->setUniform("pointLights.pos", pointLightPos[0], NUMOFLIGHTS);
	_shader.get()->setUniform("pointLights.color", pointLightColor[0], NUMOFLIGHTS);
	_shader.get()->setUniform("pointLights.intensity", pointLightIntensity[0], NUMOFLIGHTS);
	_shader.get()->setUniform("pointLights.properties", pointLightProperties[0], NUMOFLIGHTS);




	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get() };

		drawCall->render(states);
	}

	_drawCalls.clear();
	_pointLights.clear();
}
