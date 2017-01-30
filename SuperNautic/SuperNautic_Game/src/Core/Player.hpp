#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML\Window\Event.hpp>

#include "Input.hpp"
#include "Ship.hpp"
#include "HUD.hpp"

class Player
{
public:
	Player();
	Player(int id);
	~Player();

	void update(float dt);
	void render();
	void renderHUD(sf::RenderTarget & target, sf::RenderStates states);

private:
	Input _input;
	int _playerId;
	Ship _ship;
	HUD _hud;
};

#endif // !PLAYER_HPP
