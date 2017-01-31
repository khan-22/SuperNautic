#include "Player.hpp"

Player::Player() :
	_playerId(0),
	_input(0),
	_hud(1280, 720)
{
}

Player::Player(int id) :
	_playerId(id),
	_input(id),
	_hud(1280, 720)
{
}

Player::~Player()
{
}


void Player::render()
{
    // ...
}

void Player::renderHUD(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(_hud);
}

void Player::update(float dt)
{
    if(_input.checkActive())
    {
        _ship.steer(_input.getLeftStickXValue() * dt);
        _ship.accelerate(_input.getTriggersValue() * dt);
    }
    else
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            _ship.steer(-1.f * dt);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            _ship.steer(1.f * dt);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            _ship.accelerate(1.f * dt);

        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            _ship.accelerate(-1.f * dt);
        }
    }

    _ship.update(dt);

    _hud.setHeat(_ship.getEngineTemperature());
    _hud.setSpeed(_ship.getSpeed());
	_hud.update();
}
