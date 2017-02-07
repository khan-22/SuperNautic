#pragma once

#ifndef RENDERABLE_3D_HPP
#define RENDERABLE_3D_HPP

#include <glm/mat4x4.hpp>
#include "Shaders/Shader.hpp"
#include "../Core/Camera.h"

namespace GFX
{
	struct RenderStates
	{
		Camera*		camera;
		glm::mat4	transform;
		Shader*		shader;
	};

	class Renderable3D
	{
	public:
		virtual ~Renderable3D();
		virtual void render(RenderStates& states) = 0;

	private:
		friend class ForwardRenderer;
		friend class DeferredRenderer;

	};
}

#endif //RENDERABLE_3D_HPP
