#include "GFX/Rendering/TransparentRenderer.hpp"

#include "GFX/Resources/TemperatureZone.hpp"
#include "Core/Utility/Camera.h"

using namespace GFX;

TransparentRenderer::TransparentRenderer()
	: _window(nullptr)
{
}

TransparentRenderer::~TransparentRenderer()
{
}

void TransparentRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_shader = ShaderCache::get("transparent_forward");
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

void TransparentRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}

void TransparentRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	glViewport(_x * windowWidth, _y * windowHeight, _width * windowWidth, _height * windowHeight);

	float uTemperatures[4] = { 1.0, 0.25, -0.25, -1.0 };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_shader.get()->bind();
	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};

		drawCall->render(states);
	}

	_drawCalls.clear();

	glDisable(GL_BLEND);

	glViewport(0, 0, windowWidth, windowHeight);
}
