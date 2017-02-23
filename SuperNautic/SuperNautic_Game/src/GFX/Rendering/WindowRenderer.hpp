#pragma once

#ifndef WINDOW_RENDERER_HPP
#define WINDOW_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"

class Camera;		// Forward-decl

namespace GFX
{
	struct Window; // Forward-decl

	class WindowRenderer
	{
	public:
		WindowRenderer();
		~WindowRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

		void render(Window& segmentWindow);
		void display(Camera& camera);

	protected:

	private:
		std::vector<Window*> _drawCalls;
		ShaderAsset	_shader;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;
	};
}

#endif // WINDOW_RENDERER_HPP