#include <string>

#include "HUD.hpp"
#include "AssetCache.hpp"

HUD::HUD(int windowWidth, int windowHeight) :
	SceneNode(),
	_heat(0),
	_speed(0),
	_position(0),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf")),
	_texture(AssetCache<sf::Texture, std::string>::get("heatchart.png"))
{
	float widthStep = windowWidth / 100.f;
	float heightStep = windowHeight / 100.f;

	_spriteHeat.setTexture(*_texture.get());
	_spriteHeat.setPosition(widthStep * 2, heightStep * 10);
	_spriteHeat.setScale(windowWidth / 50, windowHeight / 12.5);

	_tSpeed.setFont(*_font.get());
	_tSpeed.setFillColor(sf::Color::Red);
	_tSpeed.setPosition(widthStep * 75, heightStep);
	_tSpeed.setCharacterSize(windowWidth / 20);

	_tPosition.setFont(*_font.get());
	_tPosition.setFillColor(sf::Color::Cyan);
	_tPosition.setPosition(windowWidth / 2 - widthStep * 5, 0);
	_tPosition.setCharacterSize(windowWidth / 10);
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
	_tSpeed.setString("Speed: " + std::to_string((int)_speed));
	_tPosition.setString("#" + std::to_string(_position));
}

void HUD::renderCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(_spriteHeat);
	target.draw(_tSpeed);
	target.draw(_tPosition);
}