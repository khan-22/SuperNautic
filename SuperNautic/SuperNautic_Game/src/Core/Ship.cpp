#include <math.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/vector_angle.hpp"
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
		_facingDirection{ glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 300.0f, 10.0f },
		_upDirection{ 0.0f, 1.0f, 0.0f },
		_meshUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 50.0f, 3.0f },
		_cameraUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 10.0f, 5.0f },
		_minAcceleration{ 0.0f },
		_maxAcceleration{ 25.0f },
		_maxTurningSpeed{ 10.0f },
		_straighteningForce{ 10.0f },
		_speedResistance{ 0.001f },
		_preferredHeight{ 3.0f },
		_levitationForce{ 3.0f },
		_upResistance{ 10.0f }
{
	_shipModel = GFX::TexturedModel(ModelCache::get("ship.fbx"), MaterialCache::get("test.mat"));
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
		_currentTurningAngle += -_turningFactor * _maxTurningSpeed * (1.0f - _accelerationFactor * 0.7f) * dt;
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
		// Reset hit timer
		_timeSinceIntersection = 0.0f;

		// How much influence the surface normal will have on the rotation of the ship, decreases as distance from surface increases
		float normalWeight;

		if (ri._length <= _preferredHeight)
		{
			normalWeight = 1.0f;
		}
		else
		{
			normalWeight = (_preferredHeight / ri._length) * 0.3f;
		}

		// Update local directions
		_upDirection = glm::normalize(_upDirection * (1.0f - normalWeight) + ri._normal * normalWeight);
		_facingDirection.setTarget(glm::normalize(_trackForward - glm::dot(_trackForward, ri._normal) * ri._normal));
		_facingDirection.setBackupAxis(_upDirection);
		_facingDirection.update(dt);

		// Set rotation matrix
		setLookAt(_facingDirection(), _upDirection);

		// Set up/down velocity, for now linear to distance
		/*if (ri._length < _preferredHeight)
		{*/
			_upVelocity = _preferredHeight - ri._length;
		/*}
		else
		{
			_upVelocity = (_preferredHeight - ri._length) * _levitationForce;
		}*/
	}

	// 'Rotate' mesh up direction towards 'correct' up direction
	_meshUpDirection.setTarget(_upDirection);
	_meshUpDirection.setBackupAxis(_facingDirection());
	_meshUpDirection.update(dt);

	// Update camera up direction
	_cameraUpDirection.setTarget(_upDirection);
	_cameraUpDirection.setBackupAxis(_facingDirection());
	_cameraUpDirection.update(dt);
		
	// Ship always faces straight forward, only movement direction and mesh rotates
	// Create from mesh up direction
	glm::mat4 meshMatrix{ glm::vec4{ glm::normalize(glm::cross(_meshUpDirection(), _facingDirection())), 0.0f },
						  glm::vec4{ glm::normalize(_meshUpDirection() - glm::dot(_meshUpDirection(), _facingDirection()) * _facingDirection()), 0.0f },	// The part of _meshUpDirection that is orthogonal to _facingDirection
						  glm::vec4{ _facingDirection(), 0.0f },
						  glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };
	// Rotate to display turning
	meshMatrix = glm::rotate(meshMatrix, _currentTurningAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });

	// Move forward
	glm::vec3 velocityDirection = glm::rotate(_currentTurningAngle, _upDirection) * glm::vec4{ _facingDirection(), 0.0f };
	move(velocityDirection * _velocity * dt);

	// Move up/down
	/*if (ri._length < _preferredHeight)
	{*/
		move(_upDirection * _upVelocity);
	/*}
	else
	{
		move(_upDirection * _upVelocity * dt);
	}*/

	// Update model's matrix
	_shipModel.getModelAsset().get()->setModelMatrix(glm::translate(getPosition()) * meshMatrix * glm::scale(getScale()) * glm::translate(-getOrigin()));
	
	// Reset values to stop turning/acceleration if no input is provided
	_turningFactor = 0.0f;
	_accelerationFactor = 0.5f;
}

void Ship::jump()
{
	if (_currentJumpCooldown <= 0.0f)
	{
		glm::mat4 rotation = glm::rotate(glm::pi<float>(), _facingDirection());

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

void Ship::setForward(glm::vec3& forwardDirection)
{
	_trackForward = forwardDirection;
}

void Ship::setSegments(std::vector<SegmentInstance*> segments)
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

void Ship::setReturnPos(glm::vec3& returnPos)
{
	_returnPos = returnPos;
}