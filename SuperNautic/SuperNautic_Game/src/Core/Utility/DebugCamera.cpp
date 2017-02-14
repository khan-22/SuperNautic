#include <glm\gtx\transform.hpp>

#include "Core/Utility/DebugCamera.hpp"

//Default constructor (private)
DebugCamera::DebugCamera() : Camera()
{
	//Not used, private
}

//Real constructor
DebugCamera::DebugCamera(float fov, int viewWidth, int viewHeight
	, const glm::vec3 & pos, const glm::vec3 & viewDirection)
	: Camera(fov, viewWidth, viewHeight, pos, viewDirection)
	, _anglePitch(0.f)
	, _angleYaw(0.f)
	, _moveSpeed(10.f)
	, _bActive(false)
{
	_perspective = glm::perspective(glm::radians(_fov), (float)_viewWidth / _viewHeight, 0.1f, 10000.f);
}

//Destructor
DebugCamera::~DebugCamera()
{
	//Nothing
}

//Update
void DebugCamera::update(const float dt, sf::Window& window)
{
	if (_bActive)
	{
		//Updating pitch and yaw
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		const float mouseSpeed = 0.3f;
		_angleYaw -= ((float)(mousePos.x - _viewWidth / 2.0f)) * mouseSpeed;
		_anglePitch -= ((float)(mousePos.y - _viewHeight / 2.0f)) * mouseSpeed;
		//Preventing gimbal lock
		if (_anglePitch > 89)
		{
			_anglePitch = 89;
		}
		if (_anglePitch < -89)
		{
			_anglePitch = -89;
		}

		_angleYaw = fmod(_angleYaw, 360.f);
		sf::Mouse::setPosition(sf::Vector2i(_viewWidth / 2, _viewHeight / 2), window);
		//Calculating sin and cos
		float cosPitch = cosf(glm::radians(_anglePitch));
		float cosYaw = cosf(glm::radians(_angleYaw));
		float sinPitch = sinf(glm::radians(_anglePitch));
		float sinYaw = sinf(glm::radians(_angleYaw));

		_viewDir = glm::vec3(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw);

		glm::vec3 strafeRight = glm::vec3(-cosYaw, 0, sinYaw);

		//Movement
		glm::vec3 moveDirection(0, 0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			moveDirection += _viewDir;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			moveDirection -= strafeRight;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			moveDirection -= _viewDir;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			moveDirection += strafeRight;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			moveDirection += _up;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			moveDirection -= _up;
		}
		moveDirection *= _moveSpeed * dt;
		_pos += moveDirection;
		_view = glm::lookAt(_pos, _pos + _viewDir, _up);

		//Speed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			_moveSpeed = 2.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			_moveSpeed = 10.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			_moveSpeed = 50.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
		{
			_moveSpeed = 300.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
		{
			_moveSpeed = 1000.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
		{
			_moveSpeed = 2000.0f;
		}
	}
	//Checking if mouse should be used
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
	{
		_bActive = true;
		window.setMouseCursorVisible(false);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		_bActive = false;
		window.setMouseCursorVisible(true);
	}
}
