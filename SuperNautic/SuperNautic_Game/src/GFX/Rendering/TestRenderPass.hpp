#pragma once

#ifndef TEST_RENDER_PASS_HPP
#define TEST_RENDER_PASS_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Rendering/Framebuffer.hpp"

namespace GFX
{
	class TestRenderPass
	{
	public:
		TestRenderPass();
		~TestRenderPass();
		
		void initialize(sf::RenderWindow* window, Framebuffer origin, Framebuffer target, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

		void perform();

	protected:

	private:
		ShaderAsset	_shader;

		std::unique_ptr<VertexArrayObject> _screenQuad;

		sf::RenderWindow* _window;
		GLfloat _x;
		GLfloat _y;
		GLfloat _width;
		GLfloat _height;

		Framebuffer* _origin;
		Framebuffer* _target;

	};
}


#endif //TEST_RENDER_PASS_HPP