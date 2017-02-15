#include "Core/World/Player.hpp"

Player::Player() :
	_playerId(0),
	_input(0),
	_hud(1280, 720)
{
	_audio.playAudio(PlayerAudio::Sounds::engine);
}

Player::Player(int id) :
	_playerId(id),
	_input(id),
	_hud(1280, 720)
{
}
Player::Player(const Player& other) : Player{ other._playerId }
{

}

Player::~Player()
{
}


void Player::render(GFX::DeferredRenderer& renderer)
{
	renderer.render(_ship);
}

const sf::Drawable& Player::getHud() const
{
    return _hud;
}

void Player::update(float dt)
{
	_input.update();

	if (_input.bGetAValue())
	{
		_audio.playAudio(PlayerAudio::Sounds::hit);
	}

    if(_input.checkActive())
    {
        _ship.setTurning(_input.getLeftStickXValue());
        _ship.setAcceleration(_input.getTriggersValue());
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			_ship.jump();
		}
    }
    else
    {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			_ship.jump();
		}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            _ship.setTurning(-1.f);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            _ship.setTurning(1.f);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            _ship.setAcceleration(1.f);

        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            _ship.setAcceleration(-1.f);
        }
    }

    _ship.update(dt);

    _hud.setHeat(_ship.getEngineTemperature() / 100);
    _hud.setSpeed(_ship.getSpeed());
	_hud.update();

	_audio.setPitch(PlayerAudio::Sounds::engine, _ship.getEngineTemperature() / 100 + 1);
}