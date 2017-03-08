#include "Progression.hpp"

Progression::Progression(int windowWidth, int windowHeight, int amountOfPlayers):
	_windowHeight(windowHeight),
	_windowWidth(windowWidth),
	_amountOfPlayers(amountOfPlayers)
{
	float widthStep = windowWidth / 100.f;
	float heightStep = windowHeight / 100.f;

	_progressionOutline.setFillColor(sf::Color(0, 50, 100, 100));
	_progressionOutline.setSize(sf::Vector2f(widthStep * 80, heightStep));

	if (amountOfPlayers == 1)
	{
		_progressionOutline.setPosition(widthStep * 10, heightStep * 95);
	}
	else
	{
		_progressionOutline.setPosition(widthStep * 10, heightStep * 51);
	}

	for (int i = 0; i < amountOfPlayers; i++)
	{
		_positions.push_back(0);
		_progressionMeter.push_back(sf::RectangleShape());
		_progressionMeter[i].setSize(sf::Vector2f(widthStep / 2, heightStep * 2));
		_progressionMeter[i].setOrigin(0, heightStep * .5f);
		switch (i)
		{
		case 0:
			_progressionMeter[i].setFillColor(sf::Color(255, 0, 0, 200));
			break;
		case 1:
			_progressionMeter[i].setFillColor(sf::Color(0, 255, 0, 200));
			break;
		case 2:
			_progressionMeter[i].setFillColor(sf::Color(0, 0, 255, 200));
			break;
		case 3:
			_progressionMeter[i].setFillColor(sf::Color(255, 255, 0, 200));
			break;
		default:
			_progressionMeter[i].setFillColor(sf::Color::White);
			break;
		}
	}
}

Progression::~Progression()
{
}

void Progression::updatePositions(std::vector<float> positions)
{
	for (int i = 0; i < _positions.size(); i++)
	{
		_positions[i] = positions[i];
	}
}

void Progression::updateCurrent(float dtSeconds)
{
	for (int i = 0; i < _progressionMeter.size(); i++)
	{
		_progressionMeter[i].setPosition(_positions[i] * _progressionOutline.getGlobalBounds().width + _progressionOutline.getGlobalBounds().left, _progressionOutline.getGlobalBounds().top);
	}
}

void Progression::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_progressionOutline);

	for (sf::RectangleShape rs : _progressionMeter)
	{
		target.draw(rs);
	}
}
