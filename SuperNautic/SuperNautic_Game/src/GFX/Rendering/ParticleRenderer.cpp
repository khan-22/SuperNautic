#include "GFX/Rendering/ParticleRenderer.hpp"

#include "GFX/Rendering/Renderable3D.hpp"
#include "Core/Utility/Camera.h"

using namespace GFX;

ParticleRenderer::ParticleRenderer()
	: _window(nullptr)
{
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_shader = ShaderCache::get("particle_forward");
	_shader.get()->bind();

	_window = window;
	_x		= x;
	_y		= y;
	_width	= width;
	_height = height;
}

void ParticleRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}

void ParticleRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	glViewport(_x * windowWidth, _y * windowHeight, _width * windowWidth, _height * windowHeight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	_shader.get()->bind();

	_shader.get()->setUniform("uView", camera.getView());
	_shader.get()->setUniform("uPerspective", camera.getPerspective());
	_shader.get()->setUniform("uModel", glm::mat4(1.f));
	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};

		drawCall->render(states);
	}
	glDisable(GL_BLEND);

	_drawCalls.clear();
	glViewport(0, 0, windowWidth, windowHeight);
}
