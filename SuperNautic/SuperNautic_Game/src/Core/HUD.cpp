#include <string>

#include "HUD.hpp"
#include "AssetCache.hpp"

HUD::HUD(int windowWidth, int windowHeight) :
	SceneNode(),
	_heat(10),
	_speed(0),
	_position(0),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf")),
	_texture(AssetCache<sf::Texture, std::string>::get("heatchart.png"))
{
	_widthStep = windowWidth / 100.f;
	_heightStep = windowHeight / 100.f;

	_badRect.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 5));
	_badRect.setOutlineColor(sf::Color::Red);
	_badRect.setFillColor(sf::Color::Transparent);
	_badRect.setOutlineThickness(_heightStep);
	_badRect.setPosition(_widthStep * 2, _heightStep * 10);

	_warningRect.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 15));
	_warningRect.setOutlineColor(sf::Color::Yellow);
	_warningRect.setFillColor(sf::Color::Transparent);
	_warningRect.setOutlineThickness(_heightStep);
	_warningRect.setPosition(_widthStep * 2, _heightStep * 16);

	_closeRect.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 20));
	_closeRect.setOutlineColor(sf::Color::Blue);
	_closeRect.setFillColor(sf::Color::Transparent);
	_closeRect.setOutlineThickness(_heightStep);
	_closeRect.setPosition(_widthStep * 2, _heightStep * 32);

	_safeRect.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 40));
	_safeRect.setOutlineColor(sf::Color::Green);
	_safeRect.setFillColor(sf::Color::Transparent);
	_safeRect.setOutlineThickness(_heightStep);
	_safeRect.setPosition(_widthStep * 2, _heightStep * 53);



	_heatOutline.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 80));
	_heatOutline.setOutlineColor(sf::Color::Color(0, 50, 100, 100));
	_heatOutline.setFillColor(sf::Color::Transparent);
	_heatOutline.setOutlineThickness(_heightStep);
	_heatOutline.setPosition(_widthStep * 2, _heightStep * 9);

	_heatMeter.setSize(sf::Vector2f(_widthStep * 2, _heightStep * 50));
	_heatMeter.setFillColor(sf::Color::Blue);
	_heatMeter.setOrigin(_widthStep, _heightStep);
	_heatMeter.setPosition(_widthStep * 2, _heightStep * 90);

	_spriteHeat.setTexture(*_texture.get());
	_spriteHeat.setPosition(_widthStep * 2, _heightStep * 10);
	_spriteHeat.setScale(windowWidth / 50, windowHeight / 12.5);

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

void HUD::updateCurrent()
{
	float size = (_heat / 500) * _heightStep * 80;
	_heatMeter.setSize(sf::Vector2f(_widthStep * 2, size));
	_heatMeter.setPosition(_widthStep * 2, _heightStep * 90 - size);

	_tSpeed.setString("Speed: " + std::to_string((int)_heat));
	_tPosition.setString("#" + std::to_string(_position));
}

void HUD::renderCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	//target.draw(_spriteHeat);
	target.draw(_tSpeed);
	target.draw(_tPosition);
	//target.draw(_badRect);
	//target.draw(_warningRect);
	//target.draw(_closeRect);
	//target.draw(_safeRect);
	target.draw(_heatOutline);
	target.draw(_heatMeter);
}