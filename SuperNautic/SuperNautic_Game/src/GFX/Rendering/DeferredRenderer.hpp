#pragma once

#ifndef DEFERRED_RENDERER_HPP
#define DEFERRED_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Lighting/PointLight.hpp"
#include "GFX/Rendering/Framebuffer.hpp"

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

		std::unique_ptr<VertexArrayObject> _screenQuad;

		ModelAsset _lightVolume;

		void geometryPass(Camera& camera, GLsizei width, GLsizei height);
		void lightPass(Camera& camera, GLsizei width, GLsizei height);

	};
}

#endif //DEFERRED_RENDERER_HPP
