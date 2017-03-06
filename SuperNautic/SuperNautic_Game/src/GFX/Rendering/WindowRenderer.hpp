#pragma once

#ifndef WINDOW_RENDERER_HPP
#define WINDOW_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Rendering/Framebuffer.hpp"

class Camera;		// Forward-decl

namespace GFX
{
	struct Window;			// Forward-decl
	class Renderable3D;	// Forward-decl

	class WindowRenderer
	{
	public:
		WindowRenderer();
		~WindowRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

		void render(Window& segmentWindow);
		void render(Renderable3D& renderable);
		void display(Camera& camera);

		void outsidePass(Camera& camera);
		void windowPass(Camera& camera);

	protected:

	private:
		std::vector<Window*>		_windowDrawCalls;
		std::vector<Renderable3D*>	_drawCalls;
		ShaderAsset	_windowShader;
		ShaderAsset	_outsideShader;

		Framebuffer _outsideFrameBuffer;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;

		GLsizei _actualX;
		GLsizei _actualY;
		GLsizei _actualWidth;
		GLsizei _actualHeight;
	};
}

#endif // WINDOW_RENDERER_HPP