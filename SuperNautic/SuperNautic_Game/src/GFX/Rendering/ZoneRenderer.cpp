#include "GFX/Rendering/ZoneRenderer.hpp"

#include "GFX/Resources/TemperatureZone.hpp"
#include "Core/Utility/Camera.h"

using namespace GFX;

ZoneRenderer::ZoneRenderer()
	: _window(nullptr)
{
}

ZoneRenderer::~ZoneRenderer()
{
}

void ZoneRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer)
{
	_resultFramebuffer = resultFramebuffer;

	_shader = ShaderCache::get("zone_forward");
	_shader.get()->bind();
	_shader.get()->setSampler("uPattern", 0);

	_texturePattern = TextureCache::get("zone_texture.png");

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

void ZoneRenderer::render(TemperatureZone& temperatureZone)
{
	_drawCalls.push_back(&temperatureZone);
}
void ZoneRenderer::setFogDistance(float distance)
{
    _fogDistance = distance;
}
void ZoneRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

    _resultFramebuffer->bindWrite();


	glViewport(_actualX, _actualY, _actualWidth, _actualHeight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	_shader.get()->bind();
	_shader.get()->setUniform("uViewPos", camera.getPosition());
	_shader.get()->setUniform("uFogDistance", _fogDistance);

	_texturePattern.get()->bind(0);

	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};

		_shader.get()->setUniform("uTemperatures", drawCall->temperatures[0], 4);

		drawCall->model.get()->setModelMatrix(drawCall->modelMatrix);
		drawCall->model.get()->render(states);
	}

	_texturePattern.get()->unbind(0);

	glDisable(GL_BLEND);

	_drawCalls.clear();
}
