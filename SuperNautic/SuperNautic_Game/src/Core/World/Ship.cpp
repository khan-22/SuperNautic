#include <math.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "glm/gtx/norm.hpp"

#include "Core/World/Ship.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Geometry/RayIntersection.hpp"
#include "Core/Utility/CollisionUtility.hpp"

Ship::Ship()
	:	_destroyed{ false },
		_stopped{ false },
		_turningFactor{ 0.0f },
		_currentTurningAngle{ 0.0f },
		_accelerationFactor{ 0.5f },
		_jumpCooldown{ 1.5f },
		_currentJumpCooldown{ 0.0f },
		_engineTemperature{ 0.0f },
		_velocity{ 0.0f },
		_timeSinceIntersection{ 0.0f },
		_trackForward{ 0.0f, 0.0f, 1.0f },
		_shipForward{ 0.0f, 0.0f, 1.0f },
		_upDirection{ 0.0f, 1.0f, 0.0f },
		_meshForwardDirection{ glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 200.0f, 6.0f, false},
		_meshUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 100.0f, 6.0f, true },
		_cameraUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 15.0f, 10.0f, true },
		_cameraForwardDirection{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 100.0f, 10.0f, false },
		_meshPosition{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, 10.0f },
		_meshXZPosition{ glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 0 }, 100.0f },
		_minAcceleration{ 0.0f },
		_maxAcceleration{ 200.0f },
		_maxTurningSpeed{ 8.0f },
		_straighteningForce{ 3.0f },
		_steerStraighteningForce{ 15.0f },
		_speedResistance{ 0.005f },
		_preferredHeight{ 1.5f },
		_engineCooldown{ 0 },
		_engineOverload{ 0 },
		_engineFlashTime{ 0 },
		_bEngineFlash{ false },
		_bEngineOverload { false },
		_boundingBox{ glm::vec3{ 0.0f }, std::array<glm::vec3, 3> { glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f } },std::array<float, 3>{ 1.0f, 0.5f, 1.5f } },
		_cooldownOnObstacleCollision{ 2.0f }
{
	_shipModel = GFX::TexturedModel(ModelCache::get("ship.kmf"), MaterialCache::get("test.mat"));
}


Ship::Ship(glm::vec3 position) : Ship{}
{
	setPosition(position);
}

void Ship::render(GFX::RenderStates& states)
{
	// Update model's matrix
	_shipModel.getModelAsset().get()->setModelMatrix(_transformMatrix);

	_shipModel.render(states);
}

void Ship::update(float dt)
{
	dt = clamp(dt, 0.0f, 1.0f / 30.0f);

	handleInputs(dt);
	handleCooldowns(dt);
	handleTemperature(dt);
	rotateTowardTrackForward(dt);

	trackSurface();
	updateDirectionsAndPositions(dt);

	// Create mesh rotation matrix from mesh up and forward directions
	_meshMatrix = { glm::vec4{ glm::normalize(glm::cross(_meshUpDirection(), _meshForwardDirection())), 0.0f },
						  glm::vec4{ _meshUpDirection(), 0.0f },
						  glm::vec4{ _meshForwardDirection(), 0.0f },
						  glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };

	// Update matrix
	_transformMatrix = glm::translate(_meshPosition()) * _meshMatrix * glm::scale(getScale()) * glm::translate(-getOrigin());

	// Update bounding box
	_boundingBox.center = _meshPosition();
	_boundingBox.directions[0] = glm::normalize(glm::cross(_meshUpDirection(), _meshForwardDirection()));
	_boundingBox.directions[1] = glm::normalize(_meshUpDirection());
	_boundingBox.directions[2] = glm::normalize(_meshForwardDirection());

	float dot1 = glm::dot(_boundingBox.directions[0], _boundingBox.directions[1]);
	float dot2 = glm::dot(_boundingBox.directions[0], _boundingBox.directions[2]);
	float dot3 = glm::dot(_boundingBox.directions[2], _boundingBox.directions[1]);

	checkObstacleCollision();

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
	if (_engineCooldown < 0)
	{
		_accelerationFactor = clamp(accelerationFactor, -1.0f, 1.0f);
	}
}

float Ship::getEngineTemperature()
{
	return _engineTemperature;
}

float Ship::getSpeed()
{
	return _velocity;
}

bool Ship::getOverload(float dt)
{
	bool isWhite = false;

	if (_engineCooldown > 0)
	{
		if (_engineFlashTime < 0)
		{
			float denominator = 1.f;

			if (_engineCooldown > 1.f)
			{
				denominator = _engineCooldown;
			}

			_engineFlashTime = 0.5f / denominator;
			_bEngineFlash = !_bEngineFlash;
		}
		_engineFlashTime -= dt;
		isWhite = _bEngineFlash;
	}
	else if (_engineOverload > 0)
	{
		float denominator = 1.f;

		if (_engineOverload > 1.f)
		{
			denominator = _engineOverload;
		}

		if (_engineFlashTime > 0.2f / denominator || _engineFlashTime < 0)
		{
			_engineFlashTime = 0.2f / denominator;
			_bEngineFlash = !_bEngineFlash;
		}

		_engineFlashTime -= dt;
		isWhite = _bEngineFlash;
	}
	return isWhite;
}

bool Ship::isEngineOverload()
{
	bool isOverload = false;
	if (_bEngineOverload)
	{
		isOverload = true;
		_bEngineOverload = false;
	}
	return isOverload;
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

const glm::vec3 & Ship::getMeshForward() const
{
	return _meshForwardDirection();
}

const glm::vec3 & Ship::getMeshUp() const
{
	return _meshUpDirection();
}

glm::vec3 Ship::getVelocity() const
{
	return _velocity * _meshForwardDirection();
}

SurfaceType Ship::getSurfaceType() const
{
	return _currentSurface;
}

void Ship::checkObstacleCollision()
{
	// Check every relevant instance
	for (SegmentInstance* segment : _segmentsToTest)
	{
		// For every obstacle
		for (unsigned i = 0; i < segment->getObstacles().size(); ++i)
		{
			// For every bounding box in this obstacle
			for (unsigned j = 0; j < segment->getObstacles()[i].getBoundingBoxes().size(); ++j)
			{
				BoundingBox& localBox = segment->getObstacles()[i].getBoundingBoxes()[j];

				// Transform the bounding box to global space
				BoundingBox globalBox;
				globalBox.halfLengths = localBox.halfLengths;
				globalBox.center = segment->getObstacles()[i].getModelMatrix() * glm::vec4{ localBox.center, 1.0f };
				globalBox.directions[0] = glm::normalize(glm::vec3{ segment->getObstacles()[i].getModelMatrix() * glm::vec4{ localBox.directions[0], 0.0f } });
				globalBox.directions[1] = glm::normalize(glm::vec3{ segment->getObstacles()[i].getModelMatrix() * glm::vec4{ localBox.directions[1], 0.0f } });
				globalBox.directions[2] = glm::normalize(glm::vec3{ segment->getObstacles()[i].getModelMatrix() * glm::vec4{ localBox.directions[2], 0.0f } });

				// Test ship bounding box against obstacle's bounding box in global space
				if (bTestCollision(_boundingBox, globalBox))
				{
					obstacleCollision();
				}
			}
		}
	}
}
const BoundingBox & Ship::getBoundingBox() const
{
	return _boundingBox;
}

void Ship::obstacleCollision()
{
	if (_cooldownOnObstacleCollision > _engineCooldown)
	{
		_engineCooldown = _cooldownOnObstacleCollision;
	}
}

void Ship::handleInputs(float dt)
{
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

	if (_velocity < 20)
	{
		_velocity = 20;
	}
}

void Ship::handleCooldowns(float dt)
{
	// Update intersection timer
	_timeSinceIntersection += dt;

	_engineCooldown -= dt;

	// Reset ship if escaped
	if (_timeSinceIntersection > 0.4f)
	{
		setPosition(_returnPos);
		_velocity = 0.0f;
	}

	// Update jump cooldown
	if (_currentJumpCooldown > 0.0f)
	{
		_currentJumpCooldown -= dt;
	}
}

void Ship::handleTemperature(float dt)
{
	// Update engine temperature
	_engineTemperature = ((_accelerationFactor + _velocity) / 2);

	if (_engineTemperature > 80)
	{
		_engineOverload += ((_engineTemperature - 80.f) / 20.f) * dt;
		if (rand() % 500 + 1 < _engineOverload)
		{
			_engineCooldown = _engineOverload * _engineTemperature / 50;
			_bEngineFlash = true;
			_bEngineOverload = true;
		}
	}
	else
	{
		if (_engineOverload > 0)
		{
			_engineOverload -= (100 - _engineTemperature) * dt;
			if (_engineOverload < 0)
			{
				_engineOverload = 0;
			}
		}
	}
}

void Ship::rotateTowardTrackForward(float dt)
{
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
}

void Ship::updateDirectionsAndPositions(float dt)
{
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
	_meshUpDirection.setBackupAxis(_meshForwardDirection());
	_meshUpDirection.setTarget(glm::rotate(-_currentTurningAngle * 1.5f, _shipForward) * glm::vec4{ _upDirection, 0.0f });
	_meshUpDirection.update(dt);

	// Update camera up direction
	_cameraUpDirection.setBackupAxis(_meshForwardDirection());
	_cameraUpDirection.setTarget(_upDirection);
	_cameraUpDirection.update(dt);

	// Update camera forward direction
	_cameraForwardDirection.setTarget(_shipForward);
	_cameraForwardDirection.setBackupAxis(_upDirection);
	_cameraForwardDirection.update(dt);

	// Update forward/right position 
	_meshXZPosition.setTarget(getPosition());
	_meshXZPosition.update(dt);

	// Move ship mesh to correct forward/right position, keep up position
	_meshPosition.setVector(_meshPosition() + (_meshXZPosition() - _meshPosition()) - glm::dot(_upDirection, (_meshXZPosition() - _meshPosition())) * _upDirection);
}

void Ship::trackSurface()
{
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

		// Set current surface
		_currentSurface = atShipIntersection._surface;

		// Update local directions
		_upDirection = atShipIntersection._normal;
		_shipForward = glm::normalize(aheadOfShipIntersection._position - atShipIntersection._position);

		// Move up/down to the correct track height
		move(_upDirection * (_preferredHeight - (((atShipIntersection._length + aheadOfShipIntersection._length) / 2.0f) - _rayHeight)));
	}
}
