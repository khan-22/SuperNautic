#include "GFX/Rendering/WindowRenderer.hpp"

#include "GFX/Resources/Window.hpp"
#include "Core/Utility/Camera.h"

using namespace GFX;

WindowRenderer::WindowRenderer()
	: _window(nullptr)
{
}

WindowRenderer::~WindowRenderer()
{
}

void WindowRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_shader = ShaderCache::get("window_forward");
	_shader.get()->bind();
    _shader.get()->setSampler("uDiffuse", 0);
    _shader.get()->setSampler("uSpecular", 1);
    _shader.get()->setSampler("uNormal", 2);

	_window = window;
	_x		= x;
	_y		= y;
	_width	= width;
	_height = height;
}

void WindowRenderer::render(GFX::Window& segmentWindow)
{
	_drawCalls.push_back(&segmentWindow);
}

void WindowRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	glViewport(_x * windowWidth, _y * windowHeight, _width * windowWidth, _height * windowHeight);

	glm::mat4 VP = camera.getVP();

	Shader* shader = _shader.get();
	shader->bind();
	shader->setUniform("uCameraPos", camera.getPosition());

	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};

		drawCall->windowModel.get()->setModelMatrix(drawCall->modelTransform);
		drawCall->windowModel.get()->render(states);
	}

	_drawCalls.clear();
	glViewport(0, 0, windowWidth, windowHeight);
}
