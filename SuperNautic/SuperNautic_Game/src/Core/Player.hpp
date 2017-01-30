#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML\Window\Event.hpp>

#include "Input.hpp"
#include "Ship.hpp"

class Player
{
public:
	Player();
	Player(int id);
	~Player();

	void update(float dt);
	void render();

private:
	Input _input;
	int _playerId;
	Ship _ship;

};

#endif // !PLAYER_HPP
