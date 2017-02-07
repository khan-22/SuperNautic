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
	bool		_destroyed{ false };
	bool		_stopped{ false };
	float		_turningFactor{ 0.0f };
	float		_currentTurningAngle{ 0.0f };
	float		_accelerationFactor{ 0.5f };	// How hard the engine is currently working
	float		_upAcceleration{ 0.0f };		// Acceleration away/towards track
	float		_jumpCooldown{ 2.0f };			// Cooldown when jump is activated
	float		_currentJumpCooldown{ 0.0f };	// Current cooldown
	float		_engineTemperature{ 0.0f };
	float		_velocity{ 0.0f };				// Current forward velocity
	float		_upVelocity{ 0.0f };			// Current up/down velocity

	glm::vec3	_trackForward{ 0.0f, 0.0f, 1.0f };		// Forward direction of track
	glm::vec3	_facingDirection{ 0.0f, 0.0f, 1.0f };	// Current facing direction
	glm::vec3	_upDirection{ 0.0f, 1.0f, 0.0f };		// Current up direction
	glm::vec3	_meshUpDirection{ 0.0f, 1.0f, 0.0f };	// Up direction of ship mesh

	const float _minAcceleration{ 50.0f };
	const float _maxAcceleration{ 150.0f };
	const float _maxTurningSpeed{ 1.0f };
	const float _straighteningForce{ 0.5f };	// How strongly the ship turns toward track's forward direction
	const float _speedResistance{ 0.01f };		// 'Air resistance'
	const float _preferredHeight{ 2.0f };		// Desired height above track
	const float	_levitationForce{ 10.0f };		// How fast the ship accelerates toward preferred height
	const float _upResistance{ 10.0f };			// 'Air resistance' for up/down motion

	// SHIP TESTING
	const Segment* _segment;
	///////////////
};



#endif // SHIP_HPP
