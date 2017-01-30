#include "Player.hpp"

Player::Player() :
	_playerId(0),
	_input(0)
{
}

Player::Player(int id) :
	_playerId(id),
	_input(id)
{
}

Player::~Player()
{
}

std::list<sf::Event> Player::getEvents()
{
	return _input.getEvents();
}
