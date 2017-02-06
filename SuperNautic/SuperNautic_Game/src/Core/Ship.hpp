#pragma once

#ifndef SHIP_HPP
#define SHIP_HPP

#include "../GFX/Transformable3D.hpp"
#include "Utility/Utilities.hpp"

class Ship : public GFX::Transformable3D
{
public:
	Ship();
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


private:
	bool		_destroyed{ false };
	bool		_stopped{ false };
	float		_forwardVelocity{ 0.0f };
	float		_turningFactor{ 0.0f };
	float		_currentTurningAngle{ 0.0f };
	float		_accelerationFactor{ 0.5f };	// How hard the engine is currently working
	float		_upAcceleration{ 0.0f };		// Acceleration away/towards track

	float		_jumpCooldown{ 2.0f };			// Cooldown when jump is activated
	float		_currentJumpCooldown{ 0.0f };	// Current cooldown

	const float _minAcceleration{ 5.0f };
	const float _maxAcceleration{ 15.0f };
	const float _maxTurningSpeed{ 10.0f };		// Maximum degrees/s
	const float _straighteningForce{ 10.0f };	// How strongly the ship turns toward track's forward direction
	const float _speedResistance{ 10.0f };		// 'Air resistance'
	const float _preferredHeight{ 2.0f };		// Desired height above track
	const float	_levitationForce{ 10.0f };		// How fast the ship accelerates toward preferred height
	const float _upResistance{ 10.0f };			// 'Air resistance' for up/down motion
};



#endif // SHIP_HPP
