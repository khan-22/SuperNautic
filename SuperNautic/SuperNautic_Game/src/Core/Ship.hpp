#pragma once

#ifndef SHIP_HPP
#define SHIP_HPP

#include "../GFX/Transformable3D.hpp"
#include "Utility/Utilities.hpp"
#include "Track/Segment.hpp"
#include "LoadAssetFunctions.hpp"
#include "../GFX/Model.hpp"

class Ship : public GFX::Transformable3D
{
public:
	Ship();
	Ship(const Segment* segment);
	Ship(glm::vec3 position);

	void render();
	void update(float dt);

	void start() { _stopped = false; }
	void stop() { _stopped = true; }
	void destroy() { _destroyed = true; }
	void repair() { _destroyed = false; }

	// [-1..1]
	void setTurning(float turnFactor) { _turningFactor = clamp(turnFactor, -1.0f, 1.0f); }

	// [0..1]
	void setAcceleration(float accelerationFactor) 
	{ 
		_accelerationFactor = clamp(accelerationFactor, 0.0f, 1.0f);
	}

	// [0..1]
	float getEngineTemperature()
	{
		return _engineTemperature;
	}

	float getSpeed()
	{
		return _velocity;
	}

	// Sets the direction that counts as forward
	void setForward(glm::vec3& forwardDirection)
	{
		_trackForward = forwardDirection;
	}

	void jump();

	ModelAsset _shipModel;

private:
	bool		_destroyed;
	bool		_stopped;
	float		_turningFactor;
	float		_currentTurningAngle;
	float		_accelerationFactor;	// How hard the engine is currently working
	float		_upAcceleration;		// Acceleration away/towards track
	float		_jumpCooldown;			// Cooldown when jump is activated
	float		_currentJumpCooldown;	// Current cooldown
	float		_engineTemperature;
	float		_velocity;				// Current forward velocity
	float		_upVelocity;			// Current up/down velocity

	glm::vec3	_trackForward;			// Forward direction of track
	glm::vec3	_facingDirection;		// Current facing direction
	glm::vec3	_upDirection;			// Current up direction
	glm::vec3	_meshUpDirection;		// Up direction of ship mesh

	const float _minAcceleration;
	const float _maxAcceleration;
	const float _maxTurningSpeed;
	const float _straighteningForce;	// How strongly the ship turns toward track's forward direction
	const float _speedResistance;		// 'Air resistance'
	const float _preferredHeight;		// Desired height above track
	const float	_levitationForce;		// How fast the ship accelerates toward preferred height
	const float _upResistance;			// 'Air resistance' for up/down motion

	// SHIP TESTING
	const Segment* _segment;
	///////////////
};



#endif // SHIP_HPP
