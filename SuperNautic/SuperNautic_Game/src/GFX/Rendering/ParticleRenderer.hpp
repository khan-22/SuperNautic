#pragma once

#ifndef PARTICLE_RENDERER_HPP
#define PARTICLE_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"

class Camera;		// Forward-decl

namespace GFX
{
	class Renderable3D; // Forward-decl

	class ParticleRenderer
	{
	public:
		ParticleRenderer();
		~ParticleRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

		void render(Renderable3D& renderable);
		void display(Camera& camera);

	protected:

	private:
		std::vector<Renderable3D*> _drawCalls;
		ShaderAsset	_shader;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
	};
}

#endif // PARTICLE_RENDERER_HPP