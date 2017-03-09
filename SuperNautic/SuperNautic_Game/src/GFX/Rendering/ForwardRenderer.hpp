#pragma once

#ifndef FORWARD_RENDERER_HPP
#define FORWARD_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"

#include "GFX/Rendering/Framebuffer.hpp"

class Camera;		// Forward-decl

namespace GFX
{
	class Renderable3D; // Forward-decl

	class ForwardRenderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer);

		void render(Renderable3D& renderable);
		void display(Camera& camera);

	protected:

	private:
		std::vector<Renderable3D*> _drawCalls;
		ShaderAsset	_shader;

		Framebuffer* _resultFramebuffer;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
	};
}

#endif // FORWARD_RENDERER_HPP