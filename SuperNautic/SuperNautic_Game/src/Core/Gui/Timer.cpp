#include <string>

#include "Core/Gui/Timer.hpp"
#include "Core/Asset/AssetCache.hpp"

Timer::Timer(int windowWidth, int windowHeight, int amountOfPlayers) :
	SceneNode(),
	_time(0),
	_font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
	_tTime.setFont(*_font.get());
	_tTime.setFillColor(sf::Color::Cyan);
	_tTime.setString("00:00:00");
	_tTime.setCharacterSize(windowWidth / 20);
	_tTime.setOrigin(_tTime.getGlobalBounds().width / 2, _tTime.getGlobalBounds().height * 1.5);

	if (amountOfPlayers == 1)
	{
		_tTime.setPosition(static_cast<float>((windowWidth / 2)), static_cast<float>(windowHeight / 10 * 9));
	}
	else
	{
		_tTime.setPosition(static_cast<float>((windowWidth / 2)), static_cast<float>(windowHeight / 2));
	}
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