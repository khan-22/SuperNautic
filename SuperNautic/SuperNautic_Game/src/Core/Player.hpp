#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Input.hpp"

class Player
{
public:
	Player();
	Player(int id);
	~Player();

	std::list<InputEvent> getEvents();
private:
	Input _input;
	int _playerId;

};

#endif // !PLAYER_HPP
