#pragma once

#ifndef PROGRESSION_HPP
#define PROGRESSION_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"

class Progression : public SceneNode
{
public:
	Progression(int windowWidth, int windowHeight, int amountOfPlayers);
	~Progression();

	void updatePositions(std::vector<float> positions);

	void updateCurrent(float dtSeconds) override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int								_windowWidth;
	int								_windowHeight;
	int								_amountOfPlayers;

	std::vector<float>				_positions;

	sf::RectangleShape				_progressionOutline;
	std::vector<sf::RectangleShape>	_progressionMeter;
};

#endif // !PROGRESSION_HPP
