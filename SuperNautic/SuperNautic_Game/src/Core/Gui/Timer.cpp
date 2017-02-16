#include <string>

#include "Core/Gui/Timer.hpp"
#include "Core/Asset/AssetCache.hpp"

Timer::Timer(int windowWidth, int windowHeight) :
	SceneNode(),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
	_tTime.setFont(*_font.get());
	_tTime.setFillColor(sf::Color::Cyan);
	_tTime.setString("00:00:00");
	_tTime.setPosition((windowWidth / 2) - 60 - (_tTime.getLocalBounds().width / 2), windowHeight / 10 * 9);
	_tTime.setCharacterSize(windowWidth / 20);

	_time = 0;
}

Timer::~Timer()
{

}

void Timer::updateTime(float dt)
{
	_time += dt;
}

void Timer::updateCurrent()
{
	int minutes = (int)_time / 60;
	int seconds = (int)_time % 60;
	int hundredths = (int)(_time * 100) % 100;

	std::string sMinutes;
	std::string sSeconds;
	std::string sHundredths;

	if (minutes < 10)
	{
		sMinutes = "0" + std::to_string(minutes);
	}
	else
	{
		sMinutes = std::to_string(minutes);
	}

	if (seconds < 10)
	{
		sSeconds = "0" + std::to_string(seconds);
	}
	else
	{
		sSeconds = std::to_string(seconds);
	}

	if (hundredths < 10)
	{
		sHundredths = "0" + std::to_string(hundredths);
	}
	else
	{
		sHundredths = std::to_string(hundredths);
	}

	_tTime.setString(sMinutes + ":" + sSeconds + ":" + sHundredths);
}

void Timer::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_tTime);
}