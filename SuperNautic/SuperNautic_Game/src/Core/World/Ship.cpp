#include <math.h>
#include <cstdlib>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "glm/gtx/norm.hpp"

#include "Core/World/Ship.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Geometry/RayIntersection.hpp"
#include "Core/Utility/CollisionUtility.hpp"

Ship::Ship(glm::vec3 color)
	:	_destroyed{ false },
		_stopped{ false },
		_turningFactor{ 0.0f },
		_currentTurningAngle{ 0.0f },
		_accelerationFactor{ 0.5f },
		_jumpCooldown{ .8f },
		_currentJumpCooldown{ 0.0f },
		_engineTemperature{ 0.0f },
		_velocity{ 0.0f },
		_timeSinceIntersection{ 0.0f },
		_trackForward{ 0.0f, 0.0f, 1.0f },
		_shipForward{ 0.0f, 0.0f, 1.0f },
		_upDirection{ 0.0f, 1.0f, 0.0f },
		_meshForwardDirection{ glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 200.0f, 6.0f, false},
		_meshUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 100.0f, 6.0f, true },
		_cameraUpDirection{ glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, 40.0f, 10.0f, true },
		_cameraForwardDirection{ glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 100.0f, 10.0f, false },
		_meshPosition{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, 5.0f },
		_meshXZPosition{ glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 0 }, 100.0f },
		_minAcceleration{ 0.0f },
		_maxAcceleration{ 100.0f },
		_maxTurningSpeed{ 20.0f },
		_straighteningForce{ 3.0f },
		_steerStraighteningForce{ 10.0f },
		_speedResistance{ 0.001f },
		_preferredHeight{ 1.5f },
		_engineCooldown{ 0 },
		_engineOverload{ 0 },
		_engineFlashTime{ 0 },
		_bEngineFlash{ false },
		_bEngineOverload { false },
		_bObstacleCollision { false },
		_boundingBox{ glm::vec3{ 0.0f }, std::array<glm::vec3, 3> { glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f } },std::array<float, 3>{ 1.0f, 0.5f, 1.5f } },
		_cooldownOnObstacleCollision{ 3.0f },
		_immunityoOnObstacleCollision{ 4.0f },
		_immunityTimer{ 0.0f },
		_blinkFrequency{ 0.1f },
		_surfaceSlope{ 0.0f, 0.0f, 0.5f },
		_rayHeight{ 5.0f },
		_rayAheadDistance{ 5.0f },
		_steeringCooldown{ 0.0f },
		_shipCollisionShake{ 80.0f, 2.0f, 28.0f, 1.0f, 1.0f, 0.2f },
		_shipColor{ color },
		_engineLight{ glm::vec3{ 0.0f }, _shipColor, 1.0f },
		_intensityOffset{ 1.0f, 1.0f, 20.0f },
		_timeUntilIntensityUpdate{ 0.0f }
{
	setPosition(0, 0, 10);
	_shipModel = GFX::TexturedModel(ModelCache::get("ship.kmf"), MaterialCache::get("test.mat"));

	_particleSystem.init(200, glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f), 0.2f, 7.f, 50.f);
	_particleSystem.start();
}

void Ship::render(GFX::RenderStates& states)
{
	// Update model's matrix
	_shipModel.getModelAsset().get()->setModelMatrix(_transformMatrix);

	// Achieves blinking effecet
	if (static_cast<int>(_immunityTimer / _blinkFrequency) % 2 == 0)
	{
		_shipModel.render(states);
	}
}

void Ship::update(float dt)
{
	dt = clamp(dt, 0.0f, 1.0f / 30.0f);

	handleInputs(dt);
	handleCooldowns(dt);
	handleTemperature(dt);
	rotateTowardTrackForward(dt);

	trackSurface(dt);
	updateDirectionsAndPositions(dt);

	_shipCollisionShake.setMagnitude(_steeringCooldown / _cooldownOnObstacleCollision);
	_shipCollisionShake.setSpeed(_steeringCooldown / _cooldownOnObstacleCollision);
	_shipCollisionShake.update(dt);

	// Create mesh rotation matrix from mesh up and forward directions
	_meshMatrix = { glm::vec4{ glm::normalize(glm::cross(_meshUpDirection(), _meshForwardDirection())), 0.0f },
						  glm::vec4{ _meshUpDirection(), 0.0f },
						  glm::vec4{ _meshForwardDirection(), 0.0f },
						  glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } };

	// Update matrix
	_transformMatrix = glm::translate(_meshPosition()) * glm::translate(glm::vec3{ _meshMatrix * glm::vec4{ _shipCollisionShake(), 0.0f } }) *_meshMatrix * glm::scale(getScale()) * glm::translate(-getOrigin());

	// Update bounding box
	_boundingBox.center = _meshPosition();
	_boundingBox.directions[0] = glm::normalize(glm::cross(_meshUpDirection(), _meshForwardDirection()));
	_boundingBox.directions[1] = glm::normalize(_meshUpDirection());
	_boundingBox.directions[2] = glm::normalize(_meshForwardDirection());

	// Update slope
	_surfaceSlope.setTarget(glm::dot(_waypointDifference, _upDirection));
	_surfaceSlope.update(dt);

	checkObstacleCollision();

	// Handle particle system and light variables
	float interpolation = powf(clamp(_engineTemperature * 0.01f, 0.1f, 0.9f), 2.0f);
	_particleSystem.setBirthColor(_shipColor * (1.0f - interpolation) + glm::vec3{ 0.3f } * interpolation);
	_engineLight.updateColor(_shipColor * (1.0f - interpolation) + glm::vec3{ 0.3f } *interpolation);

	_particleSystem.setDeathColor(glm::vec3{0.0f});
	_particleSystem.setBirthSize(powf(_velocity * 0.03f, 1.5f) * 0.1f);

	// Update random intensity offset
	_timeUntilIntensityUpdate -= dt;
	if (_timeUntilIntensityUpdate <= 0.0f)
	{
		_timeUntilIntensityUpdate += 0.02f + (static_cast<float>(rand()) / RAND_MAX) * 0.06f;
		_intensityOffset.setTarget(static_cast<float>(rand()) / RAND_MAX);
	}
	_intensityOffset.update(dt);

	_engineLight.changeIntensity(powf(_velocity * 0.03f, 1.5f) * 0.2f + _intensityOffset() * _velocity * 0.015f);

	_particleSystem.update(dt, _transformMatrix * glm::vec4{ 0.0f, 0.0f, -1.8f, 1.0f });
	_engineLight.setPosition(_transformMatrix * glm::vec4{ 0.0f, 0.0f, -1.8f, 1.0f });

	// Reset values to stop turning/acceleration if no input is provided
	_turningFactor = 0.0f;
	_accelerationFactor = 0.0f;
}

void Ship::jump()
{
	if (_currentJumpCooldown <= 0.0f && _steeringCooldown <= 0.0f && _inactiveTimer <= 0.0f)
	{
		glm::mat4 rotation = glm::rotate(glm::pi<float>(), _trackForward);

		_upDirection = rotation * glm::vec4{ _upDirection, 0.0f };

		_upDirection = glm::normalize(_upDirection - glm::dot(_upDirection, _trackForward) * _trackForward);

		_currentJumpCooldown = _jumpCooldown;
	}
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

void Ship::obstacleCollision()
{
	if (_immunityTimer <= 0.0f)
	{
		_engineCooldown = std::max(_cooldownOnObstacleCollision, _engineCooldown);
		_steeringCooldown = _cooldownOnObstacleCollision;
		_immunityTimer = _immunityoOnObstacleCollision;
		_bObstacleCollision = true;
	}
}

void Ship::rotateAtStart(float down, float angle)
{
	move(0, -down, 0);

	glm::mat4 rotation = glm::rotate(angle, glm::vec3{ 0, 0, 1 });

	setPosition(rotation * glm::vec4{ getPosition(), 0.0f });
	_upDirection = rotation * glm::vec4{ _upDirection, 0.0f };
	_meshUpDirection.setVector(_upDirection);
	_cameraUpDirection.setVector(_upDirection);
}

void Ship::handleInputs(float dt)
{
	if (!_stopped)
	{
		if (_steeringCooldown <= 0.0f && _inactiveTimer <= 0.0f)
		{
			// Update turning angle										reduce maneuverability at high acceleration
			_currentTurningAngle += -_turningFactor * _maxTurningSpeed * (1.0f - _accelerationFactor * 0.0f) * dt;
		}
		else
		{
			_accelerationFactor = -1.0f;
		}
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

	if (_velocity < 50)
	{
		_velocity = 50;
	}
	else if (_velocity > 200)
	{
		_velocity = 200;
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
		_velocity -= 0.1f * dt;
	}

	// Update jump cooldown
	if (_currentJumpCooldown > 0.0f)
	{
		_currentJumpCooldown -= dt;
	}

	// Update steering cooldown
	if (_steeringCooldown > 0.0f)
	{
		_steeringCooldown -= dt;
	}

	if (_immunityTimer > 0.0f)
	{
		_immunityTimer -= dt;
	}

	if (_inactiveTimer > 0.0f)
	{
		_inactiveTimer -= dt;
	}
}

void Ship::handleTemperature(float dt)
{
	// Update engine temperature
	float fieldAddition = 1.0f;

	if (getSurfaceTemperature() < -0.1f)
	{
		fieldAddition = 0.9f;
	}
	else if (getSurfaceTemperature() > 0.1f)
	{
		fieldAddition = 1.1f;
	}

	float enginePower = ((_accelerationFactor + _velocity) / 2) * fieldAddition;

	if (enginePower > _engineTemperature)
	{
		if (enginePower > _engineTemperature + 1)
		{
			_engineTemperature = (_engineTemperature * 24 + enginePower) / 25;
		}
		else
		{
			_engineTemperature += 1.f * dt;
		}
	}
	else
	{
		if (enginePower < _engineTemperature - 1)
		{
			_engineTemperature = (_engineTemperature * 49 + enginePower) / 50;
		}
		else
		{
			_engineTemperature -= 1.f * dt;
		}
	}

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
			_engineOverload -= (100 - _engineTemperature) / 100 * dt;
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
	glm::vec3 visualTurningDirection = glm::rotate(_currentTurningAngle * 0.5f, _upDirection) * glm::vec4{ _shipForward, 0.0f };
	move(velocityDirection * _velocity * dt);

	// Update mesh position
	_meshPosition.setTarget(getPosition());
	_meshPosition.update(dt);

	// Update mesh forward direction
	_meshForwardDirection.setTarget(visualTurningDirection);
	_meshForwardDirection.setBackupAxis(_upDirection);
	_meshForwardDirection.update(dt);

	// Update mesh up direction
	_meshUpDirection.setBackupAxis(_meshForwardDirection());
	_meshUpDirection.setTarget(glm::rotate(-_currentTurningAngle * 1.0f, _shipForward) * glm::vec4{ _upDirection, 0.0f });
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

void Ship::trackSurface(float dt)
{
	// Create rays and test for intersections
	Ray atShipRay{ getPosition() + _upDirection * _rayHeight, -_upDirection, 1000.0f };
	RayIntersection atShipIntersection{ false };

	for (unsigned i = 0; i < _segmentsToTest.size(); ++i)
	{
		RayIntersection at = _segmentsToTest[i]->rayIntersectionTest(atShipRay);

		if (at && (!atShipIntersection || at._length < atShipIntersection._length))
		{
			atShipIntersection = at;
		}
	}

	if (atShipIntersection)
	{
		// Reset hit timer
		_timeSinceIntersection = 0.0f;

		// Set current surface
		_currentSurfaceTemperature = atShipIntersection._surface;

		// Update local directions
		_upDirection = atShipIntersection._normal;
		_shipForward = glm::normalize(_shipForward - glm::dot(_shipForward, _upDirection) * _upDirection);

		// Move up/down to the correct track height
		move(_upDirection * (_preferredHeight - (atShipIntersection._length - _rayHeight)));
	}
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

		//_velocity *= 0.9999f * dt;
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

const glm::vec3 Ship::getCameraForward() const
{
	return glm::normalize(_cameraForwardDirection() - _cameraUpDirection() * 1.0f * _surfaceSlope());
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

const glm::vec3 Ship::getVelocity() const
{
	return _velocity * _meshForwardDirection();
}

const glm::vec3 Ship::getCameraPosition() const
{
	return _meshPosition() - _cameraForwardDirection() * (6.0f - abs(_surfaceSlope()) * 1.0f /*+ _velocity / 90.0f*/) + _cameraUpDirection() * (3.0f + _surfaceSlope() * 5.0f);
}

const glm::mat4 & Ship::getTransform() const
{
	return _transformMatrix;
}

float Ship::getSurfaceTemperature() const
{
	return _currentSurfaceTemperature;
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

void Ship::setWaypointDifference(const glm::vec3 & difference)
{
	_waypointDifference = difference;
}

void Ship::setSteeringCooldown(float cooldown)
{
	_steeringCooldown = cooldown;
}

void Ship::setInactiveTime(float inactiveTime)
{
	_inactiveTimer = inactiveTime;
}

float Ship::getSteeringCooldown()
{
	return _steeringCooldown;
}

bool Ship::checkIfCollided()
{
	if (_bObstacleCollision)
	{
		_bObstacleCollision = false;
		return true;
	}
	else
	{
		return false;
	}
}

GFX::ParticleSystem& Ship::getParticleSystem()
{
	return _particleSystem;
}

PointLight& Ship::getPointLight()
{
	return _engineLight;
}

const glm::vec3 & Ship::getColor()
{
	return _shipColor;
}

const BoundingBox & Ship::getBoundingBox() const
{
	return _boundingBox;
}
