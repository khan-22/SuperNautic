#include <string>

#include "Core/Gui/HUD.hpp"
#include "Core/Asset/AssetCache.hpp"

HUD::HUD(int windowWidth, int windowHeight) :
	SceneNode(),
	_heat(10),
	_speed(0),
	_position(0),
	_progression(0),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf")),
	_texture(AssetCache<sf::Texture, std::string>::get("heatchart.png"))
{
	_widthStep = windowWidth / 100.f;
	_heightStep = windowHeight / 100.f;

	_heatSizeX = _widthStep * 2;
	_heatSizeY = _heightStep * 80;

	_heatPosX = _widthStep * 3 + _offsetX;
	_heatPosY = _heightStep * 90 + _offsetY;

	_progressionPosX = windowWidth - _widthStep * 4 + _offsetX;

	_heatOutline.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 80));
	_heatOutline.setOutlineColor(sf::Color(0, 50, 100, 100));
	_heatOutline.setFillColor(sf::Color::Transparent);
	_heatOutline.setOutlineThickness(_heightStep);
	_heatOutline.setPosition(_widthStep * 2, _heightStep * 9);

	_heatMeter.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 50));
	_heatMeter.setFillColor(sf::Color::Blue);
	_heatMeter.setOrigin(_widthStep, _heightStep);
	_heatMeter.setPosition(_widthStep * 2, _heightStep * 90);

	_progressionOutline.setSize(sf::Vector2f(_widthStep, _heightStep * 80));
	_progressionOutline.setOutlineColor(sf::Color(0, 50, 100, 100));
	_progressionOutline.setFillColor(sf::Color(50, 0, 100, 100));
	//_progressionOutline.setOutlineThickness(_heightStep);
	_progressionOutline.setPosition(_progressionPosX, _heightStep * 9 + _offsetY);

	_progressionMeter.setSize(sf::Vector2f(_widthStep * 3, _heightStep / 2));
	_progressionMeter.setFillColor(sf::Color(200, 200, 200, 255));
	_progressionMeter.setOrigin(_widthStep, _heightStep);
	_progressionMeter.setPosition(_progressionPosX, _heightStep * 90 + _offsetY);

	_tSpeed.setFont(*_font.get());
	_tSpeed.setFillColor(sf::Color::Red);
	_tSpeed.setPosition(_widthStep * 75, _heightStep);
	_tSpeed.setCharacterSize(windowWidth / 20);

	_tPosition.setFont(*_font.get());
	_tPosition.setFillColor(sf::Color::Cyan);
	_tPosition.setPosition(windowWidth / 2 - _widthStep * 5, 0);
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

void HUD::setProgression(float progression)
{
	_progression = progression;
}

void HUD::setScreenSize(int width, int height, int offsetX, int offsetY)
{
	_widthStep = width / 100.f;
	_heightStep = height / 100.f;

	_offsetX = offsetX;
	_offsetY = offsetY;

	_heatSizeX = _widthStep * 2;
	_heatSizeY = _heightStep * 80;

	_heatPosX = _widthStep * 3 + _offsetX; 
	_heatPosY = _heightStep * 90 + _offsetY;

	_progressionPosX = width - (_widthStep * 4 + _offsetX);

	_heatOutline.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 80));
	_heatOutline.setOutlineThickness(_heightStep);
	_heatOutline.setPosition(_widthStep * 2 + _offsetX, _heightStep * 9 + _offsetY);

	_heatMeter.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 50));
	_heatMeter.setOrigin(_widthStep, _heightStep);
	_heatMeter.setPosition(_widthStep * 2 + _offsetX, _heightStep * 90 + _offsetY);

	_progressionOutline.setSize(sf::Vector2f(_widthStep, _heightStep * 80));
	//_progressionOutline.setOutlineThickness(_heightStep);
	_progressionOutline.setPosition(_progressionPosX, _heightStep * 9 + _offsetY);

	_progressionMeter.setSize(sf::Vector2f(_widthStep * 3, _heightStep / 2));
	_progressionMeter.setOrigin(_widthStep, _heightStep);
	_progressionMeter.setPosition(_progressionPosX, _heightStep * 90 + _offsetY);

	_tSpeed.setCharacterSize(static_cast<unsigned>(_widthStep * 5));
	_tSpeed.setPosition(_widthStep * 75 + _offsetX, _heightStep + _offsetY);

	_tPosition.setCharacterSize(static_cast<unsigned>(_widthStep * 5));
	_tPosition.setPosition(static_cast<float>(_widthStep * 50 - _widthStep * 5 + _offsetX), static_cast<float>(0 + _offsetY));
}

void HUD::setHeatWhite(bool isWhite)
{
	_bWhiteHeat = isWhite;
}

void HUD::updateCurrent()
{
	if (_bWhiteHeat)
	{
		_heatMeter.setFillColor(sf::Color::White);
	}
	else
	{
		int red = 0, green = 0;

		red = static_cast<int>(_heat * 255.f);
		green = static_cast<int>((1 - _heat) * 255.f);

		_heatMeter.setFillColor(sf::Color(red, green, 0, 255));
	}
	/*else if (_heat < .40)
	{
		_heatMeter.setFillColor(sf::Color::Green);
	}
	else if (_heat < .60)
	{
		_heatMeter.setFillColor(sf::Color::Blue);
	}
	else if (_heat < .80)
	{
		_heatMeter.setFillColor(sf::Color::Yellow);
	}
	else
	{
		_heatMeter.setFillColor(sf::Color::Red);
	}*/

	_sizeY = _heat * _heatSizeY;
	_heatMeter.setSize(sf::Vector2f(_heatSizeX, _sizeY));
	_heatMeter.setPosition(_heatPosX, _heatPosY - _sizeY);

	_progressionMeter.setPosition(_progressionPosX, _heightStep * 90 + _offsetY  - (_progression * _heightStep * 80));

	_tSpeed.setString("Speed: " + std::to_string(_speed));
	_tPosition.setString(std::to_string(_position));
}

void HUD::renderCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	//target.draw(_tSpeed);
	target.draw(_tPosition);
	target.draw(_heatOutline);
	target.draw(_heatMeter);
	target.draw(_progressionOutline);
	target.draw(_progressionMeter);
}
