#pragma once

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"


class HUD : public SceneNode
{
public:
	HUD(int windowWidth, int windowHeight);
	~HUD();

	void setHeat(float heat);
	void changeHeat(float change);
	void setSpeed(float speed);
	void setPosition(int position);

	void updateCurrent() override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float				_widthStep;
	float				_heightStep;
	float				_heat;
	float				_speed;
	int					_position;

	Asset<sf::Font>		_font;
	Asset<sf::Texture>	_texture;
	sf::RectangleShape	_badRect;
	sf::RectangleShape	_warningRect;
	sf::RectangleShape	_closeRect;
	sf::RectangleShape	_safeRect;

	sf::RectangleShape	_heatOutline;
	sf::RectangleShape	_heatMeter;

	sf::Sprite			_spriteHeat;
	sf::Text			_tSpeed;
	sf::Text			_tPosition;

};

#endif // !HUD_HPP
