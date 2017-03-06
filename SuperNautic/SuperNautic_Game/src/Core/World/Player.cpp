#include "Core/World/Player.hpp"

Player::Player(int id, glm::vec3 color) :
	_playerId(id),
	_input(id),
	_ship(color),
	_hud(1280, 720),
	_camera(90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 }),
	_fpCamera(90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 }),
	_currentCamera(&_camera),
	_cameraVelocityPositionShake(30.0f, 0.02f, 19.0f, 0.007f, 1.0f, 0.2f),
	_cameraVelocityDirectionShake(30.5f, 0.002f, 10.1f, 0.0009f, 1.0f, 0.2f),
	_cameraCollisionPositionShake(40.0f, 0.5f, 27.0f, 0.25f, 1.5f, 0.2f),
	_cameraCollisionDirectionShake(35.0f, 0.2f, 24.0f, 0.16f, 1.0f, 0.2f)
{
	_bIsFirstPerson = false;
}

Player::Player(const Player& other) : Player{ other._playerId, glm::vec3{0, 0, 0} }
{
	LOG_ERROR("Player copy constructor called! That is bad :(");
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

	// Update shake offsets
	_cameraVelocityPositionShake.setMagnitude(std::max((glm::length(_ship.getVelocity()) - 80.0f) * 0.01f, 0.0f));
	_cameraVelocityPositionShake.setSpeed(std::max((glm::length(_ship.getVelocity()) - 80.0f) * 0.01f, 0.0f));
	_cameraVelocityPositionShake.update(dt);

	_cameraVelocityDirectionShake.setMagnitude(std::max((glm::length(_ship.getVelocity()) - 80.0f) * 0.01f, 0.0f));
	_cameraVelocityDirectionShake.setSpeed(std::max((glm::length(_ship.getVelocity()) - 80.0f) * 0.01f, 0.0f));
	_cameraVelocityDirectionShake.update(dt);

	_cameraCollisionPositionShake.setMagnitude(_ship.getSteeringCooldown() / 2.f);
	_cameraCollisionPositionShake.setSpeed(_ship.getSteeringCooldown() / 2.f);
	_cameraCollisionPositionShake.update(dt);

	_cameraCollisionDirectionShake.setMagnitude(_ship.getSteeringCooldown() / 2.f);
	_cameraCollisionDirectionShake.setSpeed(_ship.getSteeringCooldown() / 2.f);
	_cameraCollisionDirectionShake.update(dt);

	if (_ship.isEngineOverload())
	{
		_audio.playAudio(_audio.overheat);
	}

	if (_ship.checkIfCollided())
	{
		_audio.playAudio(_audio.obstacleCollision);
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

	// Apply camera shake
	glm::mat4 toCameraSpace{ glm::vec4{ glm::normalize(glm::cross(_currentCamera->getUp(), _currentCamera->getDirection())), 0.0f },
							 glm::vec4{ _currentCamera->getUp(), 0.0f }, 
							 glm::vec4{ _currentCamera->getDirection(), 0.0f }, 
							 glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };
	glm::vec3 newPos = glm::vec4{ _currentCamera->getPosition(), 0.0f } + toCameraSpace * glm::vec4{ _cameraVelocityPositionShake() + _cameraCollisionPositionShake(), 0.0f };
	glm::vec3 newDir = glm::normalize(glm::vec4{ _currentCamera->getDirection(), 0.0f } + toCameraSpace * glm::vec4{ _cameraVelocityDirectionShake() + _cameraCollisionDirectionShake(), 0.0f });
	_currentCamera->setView(glm::lookAt(newPos, newPos + newDir, _currentCamera->getUp()));

	// TODO: Apply ship shake

    _hud.setSpeed(_ship.getSpeed());
	_hud.update();

	float speed = _ship.getSpeed();
	float newFOV = 90.f + 40.f * (speed - 20.f) / 180.f;

	_currentCamera->setFOV(newFOV);
	_audio.setPitch(PlayerAudio::Sounds::engine, _ship.getSpeed() / 200 + .75f);
}

Camera* Player::getCamera()
{
	return _currentCamera;
}

void Player::setPosition(int position)
{
	_hud.setPosition(position);
}

void Player::swapPerspective()
{
	_bIsFirstPerson = !_bIsFirstPerson;
}

void Player::shipCollision()
{
	_audio.playAudio(PlayerAudio::Sounds::shipCollision);
}