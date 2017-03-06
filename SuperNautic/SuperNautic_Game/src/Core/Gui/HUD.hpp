#pragma once

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"


class HUD : public SceneNode
{
public:
	HUD(int windowWidth, int windowHeight);
	~HUD();

	void setSpeed(float speed);
	void setPosition(int position);
	void setScreenSize(int width, int height, int offsetX, int offsetY);

	void updateCurrent() override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float				_widthStep;
	float				_heightStep;
	int					_offsetX;
	int					_offsetY;
	float				_speed;
	int					_position;

	Asset<sf::Font>		_font;

	sf::RectangleShape	_speedMeter;
	sf::CircleShape		_speeder;

	sf::VertexArray		_speedLine;

	sf::Text			_tSpeed;
	sf::Text			_tPosition;

};

#endif // !HUD_HPP
