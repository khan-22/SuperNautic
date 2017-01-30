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


void Player::render()
{
    // ...
}

void Player::update(float dt)
{
    _ship.steer(_input.getLeftStickXValue() * dt);
    _ship.accelerate(_input.getTriggersValue() * dt);

    _ship.update(dt);
}
