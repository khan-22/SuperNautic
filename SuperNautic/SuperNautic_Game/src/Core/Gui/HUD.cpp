#include <string>
#include <algorithm>

#include "Core/Gui/HUD.hpp"
#include "Core/Asset/AssetCache.hpp"

HUD::HUD(int windowWidth, int windowHeight) :
	SceneNode(),
	_speed(0),
	_position(0),
	_offsetX(0),
	_offsetY(0),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
	_widthStep = windowWidth / 100.f;
	_heightStep = windowHeight / 100.f;

	_speedMeter.setSize(sf::Vector2f(_widthStep * 3, _heightStep / 2));
	_speedMeter.setFillColor(sf::Color(200, 200, 200, 255));
	_speedMeter.setOrigin(_widthStep, _heightStep * 1.5f);
	_speedMeter.setPosition(_widthStep * 50 + _offsetX, _heightStep * 90 + _offsetY);

	_speeder.setRadius(_widthStep / 2);
	_speeder.setFillColor(sf::Color::Red);

	_tSpeed.setFont(*_font.get());
	_tSpeed.setFillColor(sf::Color::Red);
	_tSpeed.setPosition(_widthStep * 75, _heightStep);
	_tSpeed.setCharacterSize(windowWidth / 20);

	_tPosition.setFont(*_font.get());
	_tPosition.setFillColor(sf::Color::Cyan);
	_tPosition.setString("0");
	_tPosition.setOrigin(_tPosition.getGlobalBounds().width / 2, 0);
	_tPosition.setPosition(windowWidth / 2.0f, _heightStep);
	_tPosition.setCharacterSize(windowWidth / 10);
}

HUD::~HUD()
{
}

void HUD::setSpeed(float speed)
{
	_speed = speed;
}

void HUD::setPosition(int position)
{
	_position = position;
}

void HUD::setScreenSize(int width, int height, int offsetX, int offsetY)
{
	_widthStep = width / 100.f;
	_heightStep = height / 100.f;

	_offsetX = offsetX;
	_offsetY = offsetY;

	_speedMeter.setSize(sf::Vector2f(_widthStep * 3, _heightStep / 2));
	_speedMeter.setFillColor(sf::Color(200, 200, 200, 255));
	_speedMeter.setOrigin(_widthStep, _heightStep * 1.5f);
	_speedMeter.setPosition(_widthStep * 50 + _offsetX, _heightStep * 90 + _offsetY);

	_tSpeed.setCharacterSize(static_cast<unsigned>(_widthStep * 5));
	_tSpeed.setPosition(_widthStep * 75 + _offsetX, _heightStep + _offsetY);

	_tPosition.setCharacterSize(static_cast<unsigned>(_widthStep * 5));
	_tPosition.setOrigin(_tPosition.getGlobalBounds().width / 2, 0);
	_tPosition.setPosition(static_cast<float>(_widthStep * 50 + _offsetX), static_cast<float>(0 + _offsetY));
}

void HUD::updateCurrent()
{
	_speeder.setPosition(static_cast<float>(_offsetX) + _widthStep * 35.0f + _widthStep * 30.0f * (_speed - 50.0f) / 150.0f, _offsetY + _heightStep * 70.0f - _heightStep * 25.0f * sinf(_speed / 250.0f * 3.1415f));

	//_tSpeed.setString("Speed: " + std::to_string(_speed));
}

void HUD::renderCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(_tPosition);
	target.draw(_speeder);
}
