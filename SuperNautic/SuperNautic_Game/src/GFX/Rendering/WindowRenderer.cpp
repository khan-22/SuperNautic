#include "GFX/Rendering/WindowRenderer.hpp"

#include "GFX/Resources/Window.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
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
	_windowShader = ShaderCache::get("window_forward");
	_windowShader.get()->bind();
    _windowShader.get()->setSampler("uOutsideBuffer", 0);

	_outsideShader = ShaderCache::get("outside_forward");
	_outsideShader.get()->bind();
	_outsideShader.get()->setSampler("uDiffuse", 0);
	_outsideShader.get()->setSampler("uSpecular", 1);
	_outsideShader.get()->setSampler("uNormal", 2);
	_outsideShader.get()->setSampler("uIllumination", 3);

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

	GLuint colorChannels[] = { 3 };
	_outsideFrameBuffer.initialize(_actualWidth, _actualHeight, colorChannels, sizeof(colorChannels) / sizeof(colorChannels[0]));
}

void WindowRenderer::render(GFX::Window& segmentWindow)
{
	_windowDrawCalls.push_back(&segmentWindow);
}

void GFX::WindowRenderer::render(Renderable3D & renderable)
{
	_drawCalls.push_back(&renderable);
}

void WindowRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;


	//glm::mat4 VP = camera.getVP();

	outsidePass(camera);
	windowPass(camera);
	
	//glViewport(0, 0, windowWidth, windowHeight);
	Framebuffer::DEFAULT.bindBoth();
}

void GFX::WindowRenderer::outsidePass(Camera & camera)
{
	_outsideFrameBuffer.bindWrite();
	glViewport(0, 0, _actualWidth, _actualHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader* shader = _outsideShader.get();
	shader->bind();
	shader->setUniform("uCameraPos", camera.getPosition());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _outsideShader.get() };

		drawCall->render(states);
	}
	
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	_drawCalls.clear();

}

void GFX::WindowRenderer::windowPass(Camera & camera)
{
	Framebuffer::DEFAULT.bindWrite();
	_outsideFrameBuffer.bindRead();
	_outsideFrameBuffer.bindColorTextures();
	glViewport(_actualX, _actualY, _actualWidth, _actualHeight);

	Shader* shader = _windowShader.get();
	shader->bind();
	shader->setUniform("uCameraPos", camera.getPosition());
	shader->setUniform("uReciprocWindow", glm::vec2(1.f / _actualWidth, 1.f / _actualHeight));

	//shader->setUniform("uVP", camera.getVP());

	for (auto windowDrawCall : _windowDrawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _windowShader.get() };

		windowDrawCall->windowModel.get()->setModelMatrix(windowDrawCall->modelTransform);
		windowDrawCall->windowModel.get()->render(states);
	}

	_windowDrawCalls.clear();

}
