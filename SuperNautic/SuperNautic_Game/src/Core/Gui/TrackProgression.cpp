#include "TrackProgression.h"

TrackProgression::TrackProgression(int windowWidth, int windowHeight, int amountOfPlayers):
	_windowHeight(windowHeight),
	_windowWidth(windowWidth),
	_amountOfPlayers(amountOfPlayers)
{
	float widthStep = windowWidth / 100;
	float heightStep = windowHeight / 100;

	_progressionOutline.setFillColor(sf::Color(0, 50, 100, 100));
	_progressionOutline.setSize(sf::Vector2f(widthStep * 80, heightStep));

	if (amountOfPlayers == 1)
	{
		_progressionOutline.setPosition(widthStep * 10, heightStep * 95);
	}
	else
	{
		_progressionOutline.setPosition(widthStep * 10, heightStep * 55);
	}

	for (int i = 0; i < amountOfPlayers; i++)
	{
		_positions.push_back(0);
		_progressionMeter.push_back(sf::RectangleShape());
		_progressionMeter[i].setSize(sf::Vector2f(widthStep, heightStep));
		_progressionMeter[i].setOrigin(widthStep / 2, heightStep / 2);
		switch (i)
		{
		case 0:
			_progressionMeter[i].setFillColor(sf::Color::Red);
			break;
		case 1:
			_progressionMeter[i].setFillColor(sf::Color::Green);
			break;
		case 2:
			_progressionMeter[i].setFillColor(sf::Color::Blue);
			break;
		case 3:
			_progressionMeter[i].setFillColor(sf::Color::Yellow);
			break;
		default:
			_progressionMeter[i].setFillColor(sf::Color::White);
			break;
		}
	}
}

TrackProgression::~TrackProgression()
{
}

void TrackProgression::updatePositions(std::vector<float> positions)
{
	for (int i = 0; i < _positions.size(); i++)
	{
		_positions[i] = positions[i];
	}
}

void TrackProgression::updateCurrent()
{
	for (int i = 0; i < _progressionMeter.size(); i++)
	{
		_progressionMeter[i].setPosition(_positions[i], 0);
	}
}

void TrackProgression::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_progressionOutline);

	for (sf::RectangleShape rs : _progressionMeter)
	{
		target.draw(rs);
	}
}