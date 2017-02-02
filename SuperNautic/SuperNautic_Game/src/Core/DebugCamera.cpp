#include <SFML\Graphics.hpp>
#include <glm\gtx\transform.hpp>

#include "DebugCamera.hpp"

//Default constructor (private)
DebugCamera::DebugCamera() : Camera()
{
	//Not used, private
}

//Real constructor
DebugCamera::DebugCamera(float fov, float top, float left, int viewWidth, int viewHeight
	, const glm::vec3 & pos, const glm::vec3 & viewDirection)
	: Camera(fov, viewWidth, viewHeight, pos, viewDirection)
	, _anglePitch(0.f)
	, _angleYaw(0.f)
	, _moveSpeed(4.f)
	, _bActive(false)
{
	
}

//Destructor
DebugCamera::~DebugCamera()
{
	//Nothing
}

//Update
void DebugCamera::update(const float dt, sf::Window& window)
{
	//Updating pitch and yaw
	if (_bActive)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		const float mouseSpeed = 0.3f;
		_angleYaw -= ((float)(mousePos.x - _viewHeight / 2.0f)) * mouseSpeed;
		_anglePitch -= ((float)(mousePos.y - _viewWidth / 2.0f)) * mouseSpeed;
		//Preventing gimbal lock
		if (_anglePitch > 89)
		{
			_anglePitch = 89;
		}
		if (_anglePitch < -89)
		{
			_anglePitch = -89;
		}

		_angleYaw = fmod(_angleYaw, 360);
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
			_moveSpeed = 1.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			_moveSpeed = 4.0f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			_moveSpeed = 10.0f;
		}
	}
	//Checking if mouse should be used
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
