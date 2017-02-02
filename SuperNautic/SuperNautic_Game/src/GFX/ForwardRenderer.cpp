#include "ForwardRenderer.hpp"

#include "Renderable3D.hpp"
#include "../Core/Camera.h"

using namespace GFX;

ForwardRenderer::ForwardRenderer()
{
}

ForwardRenderer::~ForwardRenderer()
{
}

void GFX::ForwardRenderer::initialize()
{
	_shader = ShaderCache::get("forward");
}

void ForwardRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}

void ForwardRenderer::display(Camera& camera)
{
	_shader.get()->bind();
	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _shader.get()};
		
		drawCall->render(states);
	}

	_drawCalls.clear();
}
