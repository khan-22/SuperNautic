#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include <string>

#include "HUD.hpp"

HUD::HUD() :
	_heat(0),
	_speed(0),
	_position(0)
{
	_font.loadFromFile("res/arial.ttf");

	_tHeat.setFont(_font);
	_tHeat.setColor(sf::Color::Cyan);
	_tHeat.setPosition(10, 10);
	_tHeat.setScale(1, 1);

	_tSpeed.setFont(_font);
	_tSpeed.setColor(sf::Color::Cyan);
	_tSpeed.setPosition(-10, 10);
	_tSpeed.setScale(1, 1);

	_tPosition.setFont(_font);
	_tPosition.setColor(sf::Color::Cyan);
	_tPosition.setPosition(0, 10);
	_tPosition.setScale(2, 2);
}

HUD::~HUD()
{
}

void HUD::setHeat(float heat)
{
	_heat = heat;
}

void HUD::setSpeed(float speed)
{
	_speed = speed;
}

void HUD::setPosition(int position)
{
	_position = position;
}

void HUD::updateCurrent()
{
	_tSpeed.setString("Speed: " + std::to_string(_speed));
	_tHeat.setString("Heat: " + std::to_string(_heat));
	_tPosition.setString("#" + std::to_string(_position));
}

void HUD::renderCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(_tSpeed);
	target.draw(_tHeat);
	target.draw(_tPosition);
}