#pragma once

#ifndef DEFERRED_RENDERER_HPP
#define DEFERRED_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Core/LoadAssetFunctions.hpp"

#include "Framebuffer.hpp"

namespace GFX
{
	class Renderable3D; // Forward-decl

	class DeferredRenderer
	{
	public:
		DeferredRenderer();
		~DeferredRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

		void render(Renderable3D& renderable);
		void display(Camera& camera);
	protected:

	private:
		std::vector<Renderable3D*> _drawCalls;
		ShaderAsset	_geometryPassShader;
		ShaderAsset	_lightPassShader;

		Framebuffer _frameBuffer;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;

		std::unique_ptr<VertexArrayObject> _screenQuad;

		void geometryPass(Camera& camera);
		void lightPass();

	};
}

#endif //DEFERRED_RENDERER_HPP
