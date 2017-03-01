#pragma once

#ifndef TRACKPROGRESSION_HPP
#define TRACKPROGRESSION_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"

class TrackProgression : public SceneNode
{
public:
	TrackProgression(int windowWidth, int windowHeight, int amountOfPlayers);
	~TrackProgression();

	void updatePositions(std::vector<float> positions);

	void updateCurrent() override;
	void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int								_windowWidth;
	int								_windowHeight;
	int								_amountOfPlayers;

	std::vector<float>				_positions;

	sf::RectangleShape				_progressionOutline;
	std::vector<sf::RectangleShape>	_progressionMeter;
};

#endif // !TRACKPROGRESSION_HPP