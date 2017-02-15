#include <math.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "glm/gtx/norm.hpp"

#include "Core/World/Ship.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Geometry/RayIntersection.hpp"


Ship::Ship()
	:	_destroyed{ false },
		_stopped{ false },
		_turningFactor{ 0.0f },
		_currentTurningAngle{ 0.0f },
		_accelerationFactor{ 0.5f },
		_jumpCooldown{ 2.0f },
		_currentJumpCooldown{ 0.0f },
		_engineTemperature{ 0.0f },
		_velocity{ 0.0f },
		_timeSinceIntersection{ 0.0f },
		_trackForward{ 0.0f, 0.0f, 1.0f },
		_shipForward{ 0.0f, 0.0f, 1.0f },
		_upDirection{ 0.0f, 1.0f, 0.0f },
		_meshForwardDirection{ glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 200.0f, 6.0f },
		_meshUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 150.0f, 10.0f },
		_cameraUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 15.0f, 5.0f },
		_cameraForwardDirection{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 400.0f, 20.0f },
		_meshPosition{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, 250.0f },
		_minAcceleration{ 0.0f },
		_maxAcceleration{ 200.0f },
		_maxTurningSpeed{ 8.0f },
		_straighteningForce{ 3.0f },
		_steerStraighteningForce{ 15.0f },
		_speedResistance{ 0.005f },
		_preferredHeight{ 4.0f }
{
	_shipModel = GFX::TexturedModel(ModelCache::get("ship.fbx"), MaterialCache::get("test.mat"));
	setOrigin(glm::vec3{ 0.0f, 0.25f, 0.0f });
}


Ship::Ship(glm::vec3 position) : Ship{}
{
	setPosition(position);
}

void Ship::render(GFX::RenderStates& states)
{
	_shipModel.render(states);
}

void Ship::update(float dt)
{
	dt = clamp(dt, 0.0f, 0.1f);

	// Update intersection timer
	_timeSinceIntersection += dt;

	// Reset ship if escaped
	if (_timeSinceIntersection > 0.4f)
	{
		setPosition(_returnPos);
		_velocity = 0.0f;
	}

	if (!_stopped)
	{
		// Update turning angle										reduce maneuverability at high acceleration
		_currentTurningAngle += -_turningFactor * _maxTurningSpeed * (1.0f - _accelerationFactor * 0.3f) * dt;
		// abs to preserve sign of _currentTurningAngle
		_currentTurningAngle -= _steerStraighteningForce * _currentTurningAngle * abs(_currentTurningAngle) * dt;

		// Update velocity
		_velocity += (_minAcceleration + _accelerationFactor * (_maxAcceleration - _minAcceleration)) * dt;
		_velocity -= (_velocity * _velocity * _speedResistance) * dt;
	}
	else
	{
		// Break if stopped
		_accelerationFactor = 0.0f;
		_currentTurningAngle -= 0.9f * _currentTurningAngle * dt;
		_velocity -= 0.9f * _velocity * dt;
	}

	// Update jump cooldown
	if (_currentJumpCooldown > 0.0f)
	{
		_currentJumpCooldown -= dt;
	}

	if (_velocity < 50)
	{
		_velocity = 50;
	}

	// Update engine temperature
	_engineTemperature = ((_accelerationFactor + _velocity) / 2);

	// Rotate ship forward towards track forward
	glm::vec3 rotateTowards = glm::normalize(_trackForward - glm::dot(_trackForward, _upDirection) * _upDirection);
	float dot = glm::dot(_shipForward, rotateTowards);
	float angle;

	// Protect against angle == nan
	if (dot >= 1.0f)
	{
		angle = 0.0f;
	}
	else if (dot <= -1.0f)
	{
		angle = glm::pi<float>();
	}
	else
	{
		angle = acosf(dot);
	}

	// Ensure correct rotation direction
	glm::vec3 cross = glm::cross(rotateTowards, _shipForward);
	if (glm::dot(cross, _upDirection) > 0)
	{
		angle = -angle;
	}

	// Rotate direction
	glm::mat4 forwardRotation = glm::rotate(angle * _straighteningForce * dt, _upDirection);
	_shipForward = forwardRotation * glm::vec4{ _shipForward, 0 };

	// Create rays and test for intersections
	Ray atShipRay{ getPosition() + _upDirection * _rayHeight, -_upDirection, 1000.0f };
	Ray aheadOfShipRay{ getPosition() + _shipForward * _rayAheadDistance + _upDirection * _rayHeight, -_upDirection, 1000.0f };
	RayIntersection atShipIntersection{ false };
	RayIntersection aheadOfShipIntersection{ false };
	
	for (unsigned i = 0; i < _segmentsToTest.size(); ++i)
	{
		RayIntersection at = _segmentsToTest[i]->rayIntersectionTest(atShipRay);
		RayIntersection aheadOf = _segmentsToTest[i]->rayIntersectionTest(aheadOfShipRay);

		if (at && (!atShipIntersection || at._length < atShipIntersection._length))
		{
			atShipIntersection = at;
		}

		if (aheadOf && (!aheadOfShipIntersection || aheadOf._length < aheadOfShipIntersection._length))
		{
			aheadOfShipIntersection = aheadOf;
		}
	}

	if (atShipIntersection && aheadOfShipIntersection)
	{
		// Reset hit timer
		_timeSinceIntersection = 0.0f;

		// Update local directions
		_upDirection = atShipIntersection._normal;
		_shipForward = glm::normalize(aheadOfShipIntersection._position - atShipIntersection._position);
		
		// Move up/down to the correct track height
		move(_upDirection * (_preferredHeight - (atShipIntersection._length - _rayHeight)));
	}

	// Move forward
	glm::vec3 velocityDirection = glm::rotate(_currentTurningAngle, _upDirection) * glm::vec4{ _shipForward, 0.0f };
	move(velocityDirection * _velocity * dt);

	// Update mesh position
	_meshPosition.setTarget(getPosition());
	_meshPosition.update(dt);

	// Update mesh forward direction
	_meshForwardDirection.setTarget(velocityDirection);
	_meshForwardDirection.setBackupAxis(_upDirection);
	_meshForwardDirection.update(dt);

	// Update mesh up direction
	_meshUpDirection.setTarget( glm::rotate(-_currentTurningAngle * 1.5f, _shipForward) * glm::vec4{ _upDirection, 0.0f });
	_meshUpDirection.setBackupAxis(_meshForwardDirection());
	_meshUpDirection.update(dt);

	// Update camera up direction
	_cameraUpDirection.setTarget(_upDirection);
	_cameraUpDirection.setBackupAxis(_meshForwardDirection());
	_cameraUpDirection.update(dt);

	// Update camera forward direction
	_cameraForwardDirection.setTarget(_shipForward);
	_cameraForwardDirection.setBackupAxis(_upDirection);
	_cameraForwardDirection.update(dt);
		
	// Create mesh rotation matrix from mesh up and forward directions
	glm::vec3 up = glm::rotate(-_currentTurningAngle * 0.0f, _shipForward) * glm::vec4{ _meshUpDirection(), 0.0f };

	glm::mat4 meshMatrix{ glm::vec4{ glm::normalize(glm::cross(up, _meshForwardDirection())), 0.0f },
						  glm::vec4{ glm::normalize(up - glm::dot(up, _meshForwardDirection()) * _meshForwardDirection()), 0.0f },	// The part of _meshUpDirection that is orthogonal to _meshForwardDirection
						  glm::vec4{ _meshForwardDirection(), 0.0f },
						  glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };

	// Update model's matrix
	_shipModel.getModelAsset().get()->setModelMatrix(glm::translate(_meshPosition()) * meshMatrix * glm::scale(getScale()) * glm::translate(-getOrigin()));

	// Reset values to stop turning/acceleration if no input is provided
	_turningFactor = 0.0f;
	_accelerationFactor = 0.0f;
}

void Ship::jump()
{
	if (_currentJumpCooldown <= 0.0f)
	{
		glm::mat4 rotation = glm::rotate(glm::pi<float>(), _shipForward);

		_upDirection = rotation * glm::vec4{ _upDirection, 0.0f };

		_currentJumpCooldown = _jumpCooldown;
	}
}

void Ship::setTurning(float turnFactor)
{
	_turningFactor = clamp(turnFactor, -1.0f, 1.0f);
}

void Ship::setAcceleration(float accelerationFactor)
{
	_accelerationFactor = clamp(accelerationFactor, -1.0f, 1.0f);
}

float Ship::getEngineTemperature()
{
	return _engineTemperature;
}

float Ship::getSpeed()
{
	return _velocity;
}

void Ship::setForward(const glm::vec3& forwardDirection)
{
	_trackForward = forwardDirection;
}

void Ship::setSegments(const std::vector<SegmentInstance*> segments)
{
	_segmentsToTest = segments;
}

glm::vec3 Ship::getCameraUp()
{
	return _cameraUpDirection();
}

void Ship::start()
{ 
	_stopped = false;
}
void Ship::stop()
{ 
	_stopped = true;
}
void Ship::destroy()
{ 
	_destroyed = true; 
}
void Ship::repair() 
{ 
	_destroyed = false; 
}

void Ship::setReturnPos(const glm::vec3& returnPos)
{
	_returnPos = returnPos;
}

const glm::vec3& Ship::getCameraForward() const
{
	return _cameraForwardDirection();
}

const glm::vec3& Ship::getMeshPosition() const
{
	return _meshPosition();
}