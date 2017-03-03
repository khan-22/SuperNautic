#pragma once

#ifndef ZONE_RENDERER_HPP
#define ZONE_RENDERER_HPP

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/Asset/LoadAssetFunctions.hpp"

class Camera;		// Forward-decl

namespace GFX
{
	struct TemperatureZone; // Forward-decl

	class ZoneRenderer
	{
	public:
		ZoneRenderer();
		~ZoneRenderer();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height);

		void render(TemperatureZone& temperatureZone);
		void display(Camera& camera);

	protected:

	private:
		std::vector<TemperatureZone*> _drawCalls;
		ShaderAsset	_shader;

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

#endif // ZONE_RENDERER_HPP