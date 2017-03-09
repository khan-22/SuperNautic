#include "GFX/Rendering/DeferredRenderer.hpp"

#define NUMOFLIGHTS 8

using namespace GFX;

DeferredRenderer::DeferredRenderer()
{
}


DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer)
{
	_resultFramebuffer = resultFramebuffer;

	_geometryPassShader = ShaderCache::get("geometry_forward");
	_lightPassShader = ShaderCache::get("light_deferred");

	_geometryPassShader.get()->bind();
	_geometryPassShader.get()->setSampler("uDiffuse", 0);
	_geometryPassShader.get()->setSampler("uSpecular", 1);
	_geometryPassShader.get()->setSampler("uNormal", 2);
	_geometryPassShader.get()->setSampler("uIllumination", 3);

	_lightPassShader.get()->bind();
	_lightPassShader.get()->setSampler("uPosition", 0);
	_lightPassShader.get()->setSampler("uDiffuse", 1);
	_lightPassShader.get()->setSampler("uNormal", 2);

	_window = window;
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	_actualX = static_cast<GLsizei>(_x * windowWidth);
	_actualY = static_cast<GLsizei>(_y * windowHeight);
	_actualWidth = static_cast<GLsizei>(_width  * windowWidth);
	_actualHeight = static_cast<GLsizei>(_height * windowHeight);

	GLuint colorChannels[] = { 3, 4, 3 };
	_frameBuffer.initialize(static_cast<GLuint>(_width * windowWidth), static_cast<GLuint>(_height * windowHeight), colorChannels, static_cast<GLuint>(sizeof(colorChannels) / sizeof(colorChannels[0])));

	_screenQuad.initialize(x, y, width, height);
}

void DeferredRenderer::pushPointLight(PointLight & pointLight)
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

void DeferredRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}

void DeferredRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	geometryPass(camera, windowWidth, windowHeight);
//	lightPass(camera, windowWidth, windowHeight);

	Framebuffer::DEFAULT.bindBoth();
}

void GFX::DeferredRenderer::blitDepthOnto(Framebuffer& framebuffer)
{
	LOG_GL_ERRORS();
	_frameBuffer.bindRead();
	framebuffer.bindWrite();
	LOG_GL_ERRORS();

	glBlitFramebuffer(
		0, 0, _actualWidth, _actualHeight,
		_actualX, _actualY, _actualX + _actualWidth, _actualY + _actualHeight,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	LOG_GL_ERRORS();

	framebuffer.bindBoth();
	LOG_GL_ERRORS();

}

void DeferredRenderer::geometryPass(Camera& camera, GLsizei width, GLsizei height)
{
	//*************************************************//
	//*************** GEOMETRY PASS *******************//
	//*************************************************//

	Framebuffer::DEFAULT.bindWrite();

	glViewport(_actualX, _actualY, _actualWidth, _actualHeight);

	glDisable(GL_BLEND);

	_geometryPassShader.get()->bind();

	_geometryPassShader.get()->setUniform("uViewPos", camera.getPosition());


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

	_geometryPassShader.get()->setUniform("pointLights.pos", pointLightPos[0], NUMOFLIGHTS);
	_geometryPassShader.get()->setUniform("pointLights.color", pointLightColor[0], NUMOFLIGHTS);
	_geometryPassShader.get()->setUniform("pointLights.intensity", pointLightIntensity[0], NUMOFLIGHTS);
	_geometryPassShader.get()->setUniform("pointLights.properties", pointLightProperties[0], NUMOFLIGHTS);




	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _geometryPassShader.get() };

		drawCall->render(states);
	}

	_drawCalls.clear();
	_pointLights.clear();
}

void DeferredRenderer::lightPass(Camera& camera, GLsizei width, GLsizei height)
{
	//**********************************************//
	//*************** LIGHT PASS *******************//
	//**********************************************//

	_resultFramebuffer->bindWrite();
	_frameBuffer.bindRead();
	_frameBuffer.bindColorTextures();

	Shader* lpShader = _lightPassShader.get();
	lpShader->bind();
	//lpShader->setSampler("uPosition", 0);
	//lpShader->setSampler("uDiffuse", 1);
	//lpShader->setSampler("uNormal", 2);

	lpShader->setUniform("uViewPos", camera.getPosition());

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

	lpShader->setUniform("pointLights.pos", pointLightPos[0], NUMOFLIGHTS);
	lpShader->setUniform("pointLights.color", pointLightColor[0], NUMOFLIGHTS);
	lpShader->setUniform("pointLights.intensity", pointLightIntensity[0], NUMOFLIGHTS);
	lpShader->setUniform("pointLights.properties", pointLightProperties[0], NUMOFLIGHTS);



	/*for (int i = 0; i < lightCount; i++)
	{
	std::string uName = "pointLights[" + std::to_string(i) + "]";
	lpShader->setUniform(uName + ".pos",		_pointLights[i]->getPosition());
	lpShader->setUniform(uName + ".color",		_pointLights[i]->getLightProperties().diffuseColor);

	glm::vec3 properties(_pointLights[i]->getLightProperties().constant, _pointLights[i]->getLightProperties().linear, _pointLights[i]->getLightProperties().quadratic);

	lpShader->setUniform(uName + ".properties", properties);
	}*/
	_pointLights.clear();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	//glDisable(GL_CULL_FACE);

	glViewport(0, 0, width, height);
	_screenQuad.render();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

}
