#pragma once

#ifndef DEFERRED_RENDERER_HPP
#define DEFERRED_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Lighting/PointLight.hpp"
#include "GFX/Rendering/Framebuffer.hpp"

#include "GFX/Rendering/ScreenQuad.hpp"

namespace GFX
{
	class Renderable3D; // Forward-decl

	class DeferredRenderer
	{
	public:
		DeferredRenderer();
		~DeferredRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
		void pushPointLight(PointLight& pointLight);

		void render(Renderable3D& renderable);
		void display(Camera& camera);

		void blitDepthOnto(Framebuffer& framebuffer);
	protected:

	private:
		std::vector<Renderable3D*> _drawCalls;
		std::vector<PointLight*> _pointLights;
		ShaderAsset	_geometryPassShader;
		ShaderAsset	_lightPassShader;

		Framebuffer _frameBuffer;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;

		GLsizei _actualX;
		GLsizei _actualY;
		GLsizei _actualWidth;
		GLsizei _actualHeight;

		//std::unique_ptr<VertexArrayObject> _screenQuad;
		ScreenQuad _screenQuad;

		void geometryPass(Camera& camera, GLsizei width, GLsizei height);
		void lightPass(Camera& camera, GLsizei width, GLsizei height);

	};
}

#endif //DEFERRED_RENDERER_HPP
