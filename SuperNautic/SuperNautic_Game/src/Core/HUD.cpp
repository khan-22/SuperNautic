#include <string>

#include "HUD.hpp"
#include "AssetCache.hpp"

HUD::HUD(int windowWidth, int windowHeight) :
	SceneNode(),
	_heat(0),
	_speed(0),
	_position(0),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
	float widthStep = windowWidth / 10.f;
	float heightStep = windowHeight / 10.f;

	_tHeat.setFont(*_font.get());
	_tHeat.setFillColor(sf::Color::Cyan);
	_tHeat.setPosition(widthStep, heightStep);
	_tHeat.setCharacterSize(windowWidth / 10);

	_tSpeed.setFont(*_font.get());
	_tSpeed.setFillColor(sf::Color::Cyan);
	_tSpeed.setPosition(widthStep * 9, heightStep);
	_tSpeed.setCharacterSize(windowWidth / 10);

	_tPosition.setFont(*_font.get());
	_tPosition.setFillColor(sf::Color::Cyan);
	_tPosition.setPosition(widthStep * 5, heightStep);
	_tPosition.setCharacterSize(windowWidth / 5);
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