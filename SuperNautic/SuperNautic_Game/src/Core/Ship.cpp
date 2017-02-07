#include <math.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "Ship.hpp"
#include "Geometric Primitives/Ray.hpp"
#include "Geometric Primitives/RayIntersection.hpp"


Ship::Ship()
{
	_shipModel = ModelCache::get("ship.fbx");
}

Ship::Ship(const Segment* segment) : Ship{}
{
	_segment = segment;
}

Ship::Ship(glm::vec3 position) : Ship{}
{
	setPosition(position);
}

void Ship::render()
{
	
}

void Ship::update(float dt)
{
	//dt = clamp(dt, 0.0f, 0.1f);

	// Update turning angle
	_currentTurningAngle += _turningFactor * _maxTurningSpeed * dt;
																		 // abs to preserve sign of _currentTurningAngle
	_currentTurningAngle -= _straighteningForce * _currentTurningAngle * abs(_currentTurningAngle) * dt;

	// Update velocity
	_velocity += (_minAcceleration + _accelerationFactor * (_maxAcceleration - _minAcceleration)) * dt;
	_velocity -= (_velocity * _velocity * _speedResistance) * dt;

	// Update engine temperature
	_engineTemperature = _engineTemperature * 0.8f + _accelerationFactor * 0.2f;

	// Create ray and test for intersection
	Ray r{ getPosition(), -_upDirection, 1000.0f };
	RayIntersection ri = _segment->rayIntersectionTest(r);

	if (ri)
	{
		// Update local directions
		_upDirection = glm::normalize(_upDirection * 0.0f + ri._normal * 1.0f);
		_facingDirection = glm::normalize(_trackForward - glm::dot(_trackForward, ri._normal) * ri._normal);

		// Set rotation matrix
		setLookAt(_facingDirection, _upDirection);

		if (ri._length > _preferredHeight)
		{
			_upAcceleration = -_levitationForce;
		}
		else
		{
			_upAcceleration = _levitationForce;
		}
	}

	_upVelocity = _upVelocity * 0.8f + _upAcceleration * 0.2f;
	
	// Ship always faces straight forward, only movement direction and mesh rotates
	glm::mat4 meshAndVelocityMatrix = glm::rotate(getRotation(), _currentTurningAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });

	// Move forward
	move((meshAndVelocityMatrix * glm::vec4{ _facingDirection, 0.0f }) * _velocity * dt);

	// Move up/down
	move(_upDirection * _upVelocity * dt);

	// Update model's matrix
	_shipModel.get()->setModelMatrix(glm::translate(getPosition()) * meshAndVelocityMatrix * glm::scale(getScale()) * glm::translate(-getOrigin()));
	//_shipModel.get()->setModelMatrix(getTransformMatrix());

	
	// Reset values to stop turning/acceleration if no input is provided
	_turningFactor = 0.0f;
	_accelerationFactor = 0.0f;
}
