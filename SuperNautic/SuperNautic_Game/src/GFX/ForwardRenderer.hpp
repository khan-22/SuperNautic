#pragma once

#ifndef FORWARD_RENDERER_HPP
#define FORWARD_RENDERER_HPP

#include <vector>

#include "../Core/LoadAssetFunctions.hpp"

class Camera;		// Forward-decl

namespace GFX
{
	class Renderable3D; // Forward-decl

	class ForwardRenderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();

		void initialize();

		void render(Renderable3D& renderable);
		void display(Camera& camera);

	protected:

	private:
		std::vector<Renderable3D*> _drawCalls;
		ShaderAsset	_shader;

	};
}

#endif // FORWARD_RENDERER_HPP