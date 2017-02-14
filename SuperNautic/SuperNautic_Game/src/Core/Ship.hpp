#pragma once

#ifndef SHIP_HPP
#define SHIP_HPP

#include "../GFX/Transformable3D.hpp"
#include "Utility/Utilities.hpp"
#include "Track/Segment.hpp"
#include "LoadAssetFunctions.hpp"
#include "../GFX/TexturedModel.hpp"
#include "Track/SegmentInstance.hpp"
#include "../GFX/Renderable3D.hpp"
#include "Utility/SpringRotatedVector.hpp"

class Ship : public GFX::Transformable3D, public GFX::Renderable3D
{
public:
	GFX::TexturedModel _shipModel;

	Ship();
	Ship(glm::vec3 position);

	void render(GFX::RenderStates& states) override;
	void update(float dt);

	void start();
	void stop();
	void destroy();
	void repair();

	// [-1..1]
	void setTurning(float turnFactor);
	// [-1..1]
	void setAcceleration(float accelerationFactor);
	// [0..1]
	float getEngineTemperature();
	float getSpeed();
	// Sets the direction that counts as forward
	void setForward(glm::vec3& forwardDirection);
	glm::vec3 getCameraUp();
	void jump();
	void setSegments(std::vector<SegmentInstance*> segments);
	// Indicates where to return to if ship has escaped track
	void setReturnPos(glm::vec3& returnPos);


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
	float		_timeSinceIntersection;	// Time since ray intersected track

	glm::vec3			_trackForward;			// Forward direction of track
	SpringRotatedVector	_facingDirection;		// Current facing direction
	glm::vec3			_upDirection;			// Current up direction
	SpringRotatedVector	_meshUpDirection;		// Up direction of ship mesh
	SpringRotatedVector _cameraUpDirection;
	glm::vec3			_returnPos;				// Respawn position of ship

	const float _minAcceleration;
	const float _maxAcceleration;
	const float _maxTurningSpeed;
	const float _straighteningForce;	// How strongly the ship turns toward track's forward direction
	const float _speedResistance;		// 'Air resistance'
	const float _preferredHeight;		// Desired height above track
	const float	_levitationForce;		// How fast the ship accelerates toward preferred height
	const float _upResistance;			// 'Air resistance' for up/down motion

	std::vector<SegmentInstance*> _segmentsToTest;	// Segments to test intersection against
};
#endif // SHIP_HPP
