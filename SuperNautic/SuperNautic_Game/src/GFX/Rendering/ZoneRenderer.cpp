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

void ZoneRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_shader = ShaderCache::get("zone_forward");
	_shader.get()->bind();
    _shader.get()->setSampler("uDiffuse", 0);
    _shader.get()->setSampler("uSpecular", 1);
    _shader.get()->setSampler("uNormal", 2);

	_window = window;
	_x		= x;
	_y		= y;
	_width	= width;
	_height = height;

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	_actualX = _x * windowWidth;
	_actualY = _y * windowHeight;
	_actualWidth = _width  * windowWidth;
	_actualHeight = _height * windowHeight;
}

void ZoneRenderer::render(TemperatureZone& temperatureZone)
{
	_drawCalls.push_back(&temperatureZone);
}

void ZoneRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	glViewport(_x * windowWidth, _y * windowHeight, _width * windowWidth, _height * windowHeight);

	float uTemperatures[4] = { 1.0, 0.25, -0.25, -1.0 };

	_shader.get()->bind();
	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};

		_shader.get()->setUniform("uTemperatures", drawCall->temperatures[0], 4);

		drawCall->model.get()->setModelMatrix(drawCall->modelMatrix);
		drawCall->model.get()->render(states);
	}

	_drawCalls.clear();
	glViewport(0, 0, windowWidth, windowHeight);
}
