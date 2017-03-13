#pragma once

#ifndef SHIP_HPP
#define SHIP_HPP

#include <vector>

#include "Core/Utility/Utilities.hpp"
#include "Core/Track/Segment.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/Transformable3D.hpp"
#include "GFX/Resources/TexturedModel.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
#include "GFX/Resources/ParticleSystem.hpp"
#include "GFX/Lighting/PointLight.hpp"
#include "Core/Utility/SpringRotatedVector.hpp"
#include "Core/Utility/SpringTranslatedVector.hpp"
#include "Core/Geometry/BoundingBox.hpp"
#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Utility/SpringSmoothedValue.hpp"
#include "Core/Utility/ShakeOffset.hpp"

class Ship : public GFX::Transformable3D, public GFX::Renderable3D
{
public:
	GFX::TexturedModel _shipModel;

	// UNUSED CONSTRUCTOR
	//Ship(glm::vec3 color);
	Ship(glm::vec3 color, int shipId);
	Ship(glm::vec3 color, float maxAcceleration, float maxTurningSpeed, float cooldownOnObstacleCollision, float overheatTemperature, float overhearCooldown);

	void render(GFX::RenderStates& states) override;
	void update(float dt);

	void start();
	void stop();
	void destroy();
	void repair();
	bool getStopped();

	// [-1..1]
	void setTurning(float turnFactor);
	// [-1..1]
	void setAcceleration(float accelerationFactor);

	// [0..1]
	float getEngineTemperature();
	float getSpeed();
	bool getOverload(float dt);
	bool isEngineOverload();
	// Sets the direction that counts as forward
	void setForward(const glm::vec3& forwardDirection);
	glm::vec3 getCameraUp();
	void jump();
	void setSegments(const std::vector<SegmentInstance*> segments);
	// Indicates where to return to if ship has escaped track
	void setReturnPos(const glm::vec3& returnPos);
	const glm::vec3 getCameraForward() const;
	const glm::vec3& getMeshPosition() const;
	const glm::vec3& getMeshForward() const;
	const glm::vec3& getMeshUp() const;
	const glm::vec3 getVelocity() const;
	const glm::vec3 getCameraPosition() const;
	const glm::mat4& getTransform() const;
	float getSurfaceTemperature() const;
	const BoundingBox& getBoundingBox() const;
	void obstacleCollision();
	void setWaypointDifference(const glm::vec3& difference);
	void setSteeringCooldown(float cooldown);
	void setInactiveTime(float inactiveTime);
	float getSteeringCooldown();
	bool checkIfCollided();
	GFX::ParticleSystem& getParticleSystem();
	PointLight& getPointLight();
	PointLight& getWarningLight();
	const glm::vec3& getColor();
	void setBounce(const glm::vec3& bounceVector);

	GFX::ParticleSystem& getLeftParticleSystem();
	GFX::ParticleSystem& getRightParticleSystem();

	// Moves down units in -y direction, then rotates angle radians around z
	// Used to position ship at start of race
	void rotateAtStart(float down, float angle);

	float getOverheatTemperature();

private:
	int			_shipId;
	bool		_destroyed;
	bool		_stopped;
	float		_turningFactor;
	float		_currentTurningAngle;
	float		_accelerationFactor;	// How hard the engine is currently working
	float		_jumpCooldown;			// Cooldown when jump is activated
	float		_currentJumpCooldown;	// Current cooldown
	float		_engineTemperature;
	float		_engineCooldown;
	float		_overheatCooldown;
	float		_engineOverload;
	float		_engineFlashTime;
	bool		_bEngineFlash;
	bool		_bEngineOverload;
	bool		_bObstacleCollision;
	float		_velocity;				// Current forward velocity
	float		_timeSinceIntersection;	// Time since ray intersected track
	float		_steeringCooldown;		// Time until ship can be controlled
	float		_immunityTimer;			// Time until ship will be able to collide with obstacles
	float		_inactiveTimer;			// If >0, ship is inactive and cannot be controlled

	GFX::ParticleSystem			_particleSystem;
	GFX::ParticleSystem			_leftChemtrailParticleSystem;
	GFX::ParticleSystem			_rightChemtrailParticleSystem;
	PointLight					_engineLight;
	PointLight					_warningLight;

	glm::vec3					_shipColor;

	glm::vec3					_trackForward;			// Forward direction of track
	glm::vec3					_shipForward;			// Current forward direction of ship
	glm::vec3					_upDirection;			// Current up direction
	glm::vec3					_returnPos;				// Respawn position of ship
	glm::vec3					_waypointDifference;	// Difference between waypoint direcitons, used to calculate track slope

	glm::vec3					_bounceVector;			// Direction and strength of bouncing when collided with another ship
	const float					_bounceDecay;			// Rate of decay on _bounceVector

	SpringRotatedVector			_meshForwardDirection;	// Current facing direction
	SpringRotatedVector			_meshUpDirection;		// Up direction of ship mesh
	SpringRotatedVector			_cameraUpDirection;
	SpringRotatedVector			_cameraForwardDirection;

	SpringSmoothedValue			_surfaceSlope;			// Determines the height of the camera
	SpringSmoothedValue			_intensityOffset;		// Offset for engine light intensity
	SpringSmoothedValue			_meshSpringValue;		// Spring value for mesh position
	float						_timeUntilIntensityUpdate;

	SpringTranslatedVector		_meshPosition;			// Position of ship mesh in up direction

	bool						_jumping;				// True if the ship is currently jumping

	float _currentSurfaceTemperature{ 0.0f };

	ShakeOffset	_shipCollisionShake;

	BoundingBox _boundingBox;

	glm::mat4	_meshMatrix;
	glm::mat4	_transformMatrix;

	const float _minAcceleration;
	float _maxAcceleration;
	float _maxTurningSpeed;
	const float _straighteningForce;		// How strongly the velocity direction turns toward track's forward direction
	const float _steerStraighteningForce;	// How strongly the ship resists turning
	const float _speedResistance;			// 'Air resistance'
	const float _preferredHeight;			// Desired ship height above track

	const float _rayHeight;			// Height above ship of the origin of the ray used for intersection

	float _cooldownOnObstacleCollision;
	float _immunityoOnObstacleCollision;

	float _overheatTemperature;	// Temperature at which engine overheats
	const float _warningLevel;			// Temperature at which warning light starts blinking
	const float _warningLightIntensity;
	float		_warningAccumulator;	// Used for calculating warning light intensity
	float		_engineBlinkAccumulator;// Used for "blinking" particles at high temperatures

	const float _blinkFrequency;	// How fast the ship blinks when immune to obstacle collision

	const float _inactiveAtStart;	// The time from ship spawn until it can be controlled

	std::vector<SegmentInstance*> _segmentsToTest;	// Segments to test intersection against

	void checkObstacleCollision();

	// Helper functions for update(float dtSeconds)
	void handleInputs(float dt);
	void handleCooldowns(float dt);
	void handleTemperature(float dt);
	void rotateTowardTrackForward(float dt);
	void updateDirectionsAndPositions(float dt);
	void trackSurface(float dt);
	void handleLightsAndParticles(float dt);
	void setShip();
};
#endif // SHIP_HPP
