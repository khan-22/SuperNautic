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

void ParticleRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer)
{
	_resultFramebuffer = resultFramebuffer;

	_shader = ShaderCache::get("particle_forward");
	_shader.get()->bind();

	_window = window;
	_x		= x;
	_y		= y;
	_width	= width;
	_height = height;

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	_actualX = static_cast<GLsizei>(_x * windowWidth);
	_actualY = static_cast<GLsizei>(_y * windowHeight);
	_actualWidth = static_cast<GLsizei>(_width * windowWidth);
	_actualHeight = static_cast<GLsizei>(_height * windowHeight);
}

void ParticleRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}

void ParticleRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

    _resultFramebuffer->bindWrite();

	glViewport(_actualX, _actualY, _actualWidth, _actualHeight);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	_shader.get()->bind();

	_shader.get()->setUniform("uView", camera.getView());
	_shader.get()->setUniform("uPerspective", camera.getPerspective());
	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};

		drawCall->render(states);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	_drawCalls.clear();
}
