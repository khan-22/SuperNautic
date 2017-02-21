#pragma once

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"


class HUD : public SceneNode
{
public:
	HUD(int windowWidth, int windowHeight);
	~HUD();

	void setHeat(float heat);
	void setSpeed(float speed);
	void setPosition(int position);
	void setProgression(float progression);
	void setScreenSize(int width, int height, int offsetX, int offsetY);

	void updateCurrent() override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float				_widthStep;
	float				_heightStep;
	int					_offsetX;
	int					_offsetY;
	float				_sizeY;
	float				_heat;
	float				_speed;
	int					_position;
	float				_progression;
	float				_progressionPosX;
	float				_heatSizeY;
	float				_heatSizeX;
	float				_heatPosY;
	float				_heatPosX;

	Asset<sf::Font>		_font;
	Asset<sf::Texture>	_texture;

	sf::RectangleShape	_progressionOutline;
	sf::RectangleShape	_progressionMeter;

	sf::RectangleShape	_heatOutline;
	sf::RectangleShape	_heatMeter;

	sf::Text			_tSpeed;
	sf::Text			_tPosition;

};

#endif // !HUD_HPP
