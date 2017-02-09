#include <math.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/norm.hpp"
#include "Ship.hpp"
#include "Geometric Primitives/Ray.hpp"
#include "Geometric Primitives/RayIntersection.hpp"


Ship::Ship()
	:	_destroyed{ false },
		_stopped{ false },
		_turningFactor{ 0.0f },
		_currentTurningAngle{ 0.0f },
		_accelerationFactor{ 0.5f },
		_upAcceleration{ 0.0f },
		_jumpCooldown{ 2.0f },
		_currentJumpCooldown{ 0.0f },
		_engineTemperature{ 0.0f },
		_velocity{ 0.0f },
		_upVelocity{ 0.0f },
		_trackForward{ 0.0f, 0.0f, 1.0f },
		_facingDirection{ 0.0f, 0.0f, 1.0f },
		_upDirection{ 0.0f, 1.0f, 0.0f },
		_meshUpDirection{ 0.0f, 1.0f, 0.0f },
		_minAcceleration{ 5.0f },
		_maxAcceleration{ 50.0f },
		_maxTurningSpeed{ 1.0f },
		_straighteningForce{ 3.0f },
		_speedResistance{ 0.001f },
		_preferredHeight{ 2.0f },
		_levitationForce{ 10.0f },
		_upResistance{ 10.0f }
{
	_shipModel = ModelCache::get("ship.fbx");
}


Ship::Ship(glm::vec3 position) : Ship{}
{
	setPosition(position);
}

void Ship::render(GFX::RenderStates& states)
{
	_shipModel.get()->render(states);
}

void Ship::update(float dt)
{
	if (!_stopped)
	{
		// Update turning angle										reduce maneuverability at high acceleration
		_currentTurningAngle += _turningFactor * _maxTurningSpeed * (1.0f - _accelerationFactor * 0.7f) * dt;
		// abs to preserve sign of _currentTurningAngle
		_currentTurningAngle -= _straighteningForce * _currentTurningAngle * abs(_currentTurningAngle) * dt;

		// Update velocity
		_velocity += (_minAcceleration + _accelerationFactor * (_maxAcceleration - _minAcceleration)) * dt;
		_velocity -= (_velocity * _velocity * _speedResistance) * dt;
	}
	else
	{
		// Break if stopped
		_accelerationFactor = 0.0f;
		_currentTurningAngle *= 0.9f;
		_velocity *= 0.9f;
	}

	// Update jump cooldown
	if (_currentJumpCooldown > 0.0f)
	{
		_currentJumpCooldown -= dt;
	}

	// Update engine temperature
	_engineTemperature = ((_accelerationFactor + _velocity) / 2);//_engineTemperature * 0.8f + _accelerationFactor * 0.2f;

	// Create ray and test for intersection
	Ray r{ getPosition(), -_upDirection, 1000.0f };
	RayIntersection ri{ false }; 
	
	for (unsigned i = 0; i < _segmentsToTest.size(); ++i)
	{
		RayIntersection intersection = _segmentsToTest[i]->rayIntersectionTest(r);

		if (intersection && (!ri || intersection._length < ri._length))
		{
			ri = intersection;
		}
	}

	if (ri)
	{
		// How much influence the surface normal will have on the rotation of the ship, decreases as distance from surface increases
		float normalWeight;

		if (ri._length <= _preferredHeight)
		{
			normalWeight = 1.0f;
		}
		else
		{
			normalWeight = (_preferredHeight / ri._length) * (_preferredHeight / ri._length);
		}

		// Update local directions
		_upDirection = glm::normalize(_upDirection * (1.0f - normalWeight) + ri._normal * normalWeight);
		_facingDirection = glm::normalize(_trackForward - glm::dot(_trackForward, ri._normal) * ri._normal);

		// Set rotation matrix
		setLookAt(_facingDirection, _upDirection);

		// Set up/down velocity, for now linear to distance
		_upVelocity = _preferredHeight - ri._length;
	}

	// 'Rotate' mesh up direction towards 'correct' up direction
	_meshUpDirection = glm::normalize(_meshUpDirection * 0.8f + _upDirection * 0.2f);
		
	// Ship always faces straight forward, only movement direction and mesh rotates
	glm::mat4 velocityMatrix = glm::rotate(getRotation(), _currentTurningAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });

	// Create from mesh up direction
	glm::mat4 meshMatrix{ glm::vec4{ glm::cross(_meshUpDirection, _facingDirection), 0.0f },
						  glm::vec4{ _meshUpDirection - glm::dot(_meshUpDirection, _facingDirection) * _facingDirection, 0.0f },	// The part of _meshUpDirection that is orthogonal to _facingDirection
						  glm::vec4{ _facingDirection, 0.0f },
						  glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };
	// Rotate to display turning
	meshMatrix = glm::rotate(meshMatrix, _currentTurningAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });

	// Move forward
	move((velocityMatrix * glm::vec4{ _facingDirection, 0.0f }) * _velocity * dt);

	// Move up/down
	move(_upDirection * _upVelocity * dt);

	// Update model's matrix
	_shipModel.get()->setModelMatrix(glm::translate(getPosition()) * meshMatrix * glm::scale(getScale()) * glm::translate(-getOrigin()));
	
	// Reset values to stop turning/acceleration if no input is provided
	_turningFactor = 0.0f;
	_accelerationFactor = 0.5f;
}

void Ship::jump()
{
	if (_currentJumpCooldown <= 0.0f)
	{
		glm::mat4 rotation = glm::rotate(glm::pi<float>(), _facingDirection);

		_upDirection = rotation * glm::vec4{ _upDirection, 0.0f };

		_currentJumpCooldown = _jumpCooldown;
	}
}