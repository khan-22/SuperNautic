#include "Core/World/Player.hpp"

Player::Player(int id) :
	_playerId(id),
	_input(id),
	_hud(1280, 720),
	_camera(90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 }),
	_fpCamera(90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 }),
	_currentCamera(&_camera)
{
	_bIsFirstPerson = false;
}

Player::Player(const Player& other) : Player{ other._playerId }
{

}

Player::~Player()
{
}

void Player::render(GFX::DeferredRenderer& renderer) //OBSOLETE?
{
	renderer.render(_ship);
}

const sf::Drawable& Player::getHud() const
{
    return _hud;
}

void Player::setScreenSize(int screenWidth, int screenHeight, int offsetX, int offsetY)
{
	_camera.setAspectRatio(screenWidth, screenHeight);
	_hud.setScreenSize(screenWidth, screenHeight, offsetX, offsetY);
}

void Player::setId(int id) 
{
	_input.setControllerId(id);
}

void Player::update(float dt)
{
	_input.update();

	if (_input.checkActive())
	{
		_ship.setTurning(_input.getLeftStickXValue());
		_ship.setAcceleration(_input.getTriggersValue());

		for(sf::Event event : _input.getEvents())
		{
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::A:
					_ship.jump();
					break;
				case sf::Keyboard::Y:
					swapPerspective();
					break;
				case sf::Keyboard::Escape:
					//Player paused game
					break;
				default:
					break;
				}
			}
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

	// Set camera shake value
	_camera.setShake(std::max((glm::length(_ship.getVelocity()) - 100.0f) * 0.085f, 0.0f) + (_ship.getSteeringCooldown() > 0.0f ? _ship.getSteeringCooldown() * 35.0f : 0.0f));
	_camera.update(dt);

	if (_ship.isEngineOverload())
	{
		_audio.playAudio(_audio.overheat);
	}

	if (_bIsFirstPerson)
	{
		//First person
		_currentCamera->setPos(_ship.getMeshPosition() - _ship.getCameraForward() + _ship.getCameraUp() * 2.0f);
		_currentCamera->setUp(_ship.getCameraUp());
		_currentCamera->setViewDir(_ship.getCameraForward());
	}
	else
	{
		//Third person
		_currentCamera->setPos(_ship.getCameraPosition());
		_currentCamera->setUp(_ship.getCameraUp());
		_currentCamera->setViewDir(glm::normalize(_ship.getCameraForward() - _ship.getCameraUp() * 0.1f));
	}

    _hud.setHeat(_ship.getEngineTemperature() / 100);
    _hud.setSpeed(_ship.getSpeed());
	_hud.setHeatWhite(_ship.getOverload(dt));
	_hud.update();

	float speed = _ship.getSpeed();
	float newFOV = 90.f + 40.f * (speed - 20.f) / 180.f;

	_currentCamera->setFOV(newFOV);
	_audio.setPitch(PlayerAudio::Sounds::engine, _ship.getEngineTemperature() / 100 + 1);
}

Camera* Player::getCamera()
{
	return _currentCamera;
}

void Player::setProgression(float progression)
{
	_hud.setProgression(progression);
}

void Player::setPosition(int position)
{
	_hud.setPosition(position);
}

void Player::swapPerspective()
{
	_bIsFirstPerson = !_bIsFirstPerson;
}
