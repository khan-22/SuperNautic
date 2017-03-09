#include <string>
#include <algorithm>

#include "Core/Gui/HUD.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"

//const glm::vec4 HUD::_MIN_SPEEDER_COLOR(255, 200, 100, 200);
const glm::vec4 HUD::_MIN_SPEEDER_COLOR(150, 150, 200, 200);
const glm::vec4 HUD::_MAX_SPEEDER_COLOR(255, 0, 0, 200);
//const glm::vec4 HUD::_MAX_SPEEDER_COLOR(255, 255, 255, 200);

HUD::HUD(int windowWidth, int windowHeight) :
	SceneNode(),
	_speed(0),
	_offsetX(0),
	_offsetY(0),
	_position(0),
	_speedLine(sf::LineStrip, 150),
	_overHeatTimer(0.f),
	_heatOverlayTexture(SFMLTextureCache::get("heatoverlay.png")),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
	_widthStep = windowWidth / 300.f;
	_heightStep = windowHeight / 300.f;

	_speeder.setFillColor(sf::Color::Transparent);
//	_speeder.setOutlineColor(sf::Color::Black);
	_speeder.setOutlineColor(sf::Color(125, 125, 125, 255));

	for (size_t i = 0; i < _speedLine.getVertexCount(); i++)
	{
		_speedLine[i].color = sf::Color(0, 100, 200, 100);
	}

	_heatOverlay.setTexture(_heatOverlayTexture.get());

	_tPosition.setFont(*_font.get());
	_tPosition.setFillColor(sf::Color::Cyan);
	_tPosition.setString("0");
}

HUD::~HUD()
{
}

void HUD::setSpeed(float speed)
{
	_speed = speed;
}

void HUD::setHeat(float heat)
{
	_heat = heat;
}

void HUD::setPosition(int position)
{
	_position = position;
	_tPosition.setString(std::to_string(_position));
}

void HUD::setScreenSize(int width, int height, int offsetX, int offsetY)
{
//	_widthStep = width / 100.f;
//	_heightStep = height / 100.f;
	_widthStep = width / 300.f;
	_heightStep = height / 300.f;

	_offsetX = offsetX;
	_offsetY = offsetY;


	//_speedMeter.setSize(sf::Vector2f(_widthStep * 3, _heightStep / 2));
	//_speedMeter.setOrigin(_widthStep, _heightStep * 1.5f);
	//_speedMeter.setPosition(_widthStep * 50 + _offsetX, _heightStep * 90 + _offsetY);

	//_tSpeed.setCharacterSize(static_cast<unsigned>(_widthStep * 5));
	//_tSpeed.setPosition(_widthStep * 75 + _offsetX, _heightStep + _offsetY);

	float speederRadius = ((1920.f / 2.f) / 300.f) * (6.f / 4.f);
	_speeder.setRadius(speederRadius);
	_speeder.setOutlineThickness(speederRadius / 4.f);
//	_speeder.setOrigin(_widthStep * 6.f / (_speeder.getRadius() + _speeder.getOutlineThickness()), _widthStep * 6.f / (_speeder.getRadius() + _speeder.getOutlineThickness()));
    _speeder.setOrigin(_speeder.getGlobalBounds().width / 2.f, _speeder.getGlobalBounds().height / 2.f);

	for (size_t i = 0; i < _speedLine.getVertexCount(); i++)
	{
//		_speedLine[i].position = sf::Vector2f(static_cast<float>(_offsetX) + _widthStep * 35.0f + _widthStep * 30.0f * (i) / 150.0f, _offsetY + _heightStep * 70.0f - _heightStep * 25.0f * sinf((i + 50) / 250.0f * 3.1415f));
//		_speedLine[i].position = sf::Vector2f(static_cast<float>(_offsetX) + _widthStep * 30.0f * (i) / 150.0f, _offsetY - _heightStep * 25.0f * sinf((i + 50) / 250.0f * 3.1415f));
		_speedLine[i].position = sf::Vector2f(_widthStep * 30.0f * (i) / 150.0f, -_heightStep * 25.0f * sinf((i + 50) / 250.0f * 3.1415f));
	}

	sf::FloatRect speedLineBounds = _speedLine.getBounds();
    setOrigin(speedLineBounds.width / 2.f, speedLineBounds.height / 2.f);
    sf::Transformable::setPosition(sf::Vector2f(offsetX + width / 2.f, offsetY + height * 3.f / 5.f));


	_heatOverlay.setSize(sf::Vector2f(width, height));
	_heatOverlay.setPosition(sf::Vector2f(offsetX, offsetY));

	_tPosition.setCharacterSize(static_cast<unsigned>(_widthStep * 30));
	_tPosition.setOrigin(_tPosition.getGlobalBounds().width / 2, _tPosition.getGlobalBounds().height / 2);
	_tPosition.setPosition(static_cast<float>(_widthStep * 150 + _offsetX), static_cast<float>(_heightStep * 150 + _offsetY));
}

void HUD::updateCurrent(float dtSeconds)
{
//	_speeder.setPosition(static_cast<float>(_offsetX) + _widthStep * 35.0f + _widthStep * 30.0f * (_speed - 50.0f) / 150.0f, _offsetY + _heightStep * 70.0f - _heightStep * 25.0f * sinf(_speed / 250.0f * 3.1415f));
//	_speeder.setPosition(static_cast<float>(_offsetX) + _widthStep * 30.0f * (_speed - 50.0f) / 150.0f, _offsetY - _heightStep * 25.0f * sinf(_speed / 250.0f * 3.1415f));
	_speeder.setPosition(_widthStep * 30.0f * (_speed - 50.0f) / 150.0f, -_heightStep * 25.0f * sinf(_speed / 250.0f * 3.1415f));


    float colorModifier = std::pow(_heat, 6.f);
	glm::vec4 speederColor = _MIN_SPEEDER_COLOR * (1.f - colorModifier) + _MAX_SPEEDER_COLOR * colorModifier;
//	_speeder.setFillColor(sf::Color(speederColor.r, speederColor.g, speederColor.b, speederColor.a));
//	_speeder.setFillColor(sf::Color(speederColor.r, speederColor.g, speederColor.b, _heat * 255.f));
	_speeder.setFillColor(sf::Color(_MAX_SPEEDER_COLOR.r, _MAX_SPEEDER_COLOR.g, _MAX_SPEEDER_COLOR.b, colorModifier * 255.f));

	if(_heat > 0.9f)
    {
        _overHeatTimer += dtSeconds;

        float flipsPerSecond = 3.f;

        if(size_t(_overHeatTimer * flipsPerSecond) % 2 == 0)
        {
            _speeder.setFillColor(sf::Color::White);
        }
    }
    else
    {
        _overHeatTimer = 0.f;
    }

	float trans = 0.f;

	if (_heat > .8f)
	{
		trans = (_heat - 0.8) * 5 * 255;
	}

	_heatOverlay.setFillColor(sf::Color(255, 255, 255, trans));
}

void HUD::renderCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (_position == 0)
	{
		target.draw(_speedLine, getWorldTransform());;
		target.draw(_speeder, getWorldTransform());
		target.draw(_speedLine);
		target.draw(_speeder);
		target.draw(_heatOverlay);
	}
	else
	{
		target.draw(_tPosition);
	}
}
