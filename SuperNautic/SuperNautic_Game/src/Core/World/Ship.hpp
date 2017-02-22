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
#include "Core/Utility/SpringRotatedVector.hpp"
#include "Core/Utility/SpringTranslatedVector.hpp"
#include "Core/Geometry/BoundingBox.hpp"

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
	bool getOverload(float dt);
	bool isEngineOverload();
	// Sets the direction that counts as forward
	void setForward(const glm::vec3& forwardDirection);
	glm::vec3 getCameraUp();
	void jump();
	void setSegments(const std::vector<SegmentInstance*> segments);
	// Indicates where to return to if ship has escaped track
	void setReturnPos(const glm::vec3& returnPos);
	const glm::vec3& getCameraForward() const;
	const glm::vec3& getMeshPosition() const;
	const glm::vec3& getMeshForward() const;
	SurfaceType getSurfaceType() const;
	const BoundingBox& getBoundingBox() const;
	void setEngineCooldown(float cooldown);

	// TEST
	glm::mat4 getMatrix()
	{
		return _transformMatrix;
	}
	///////

private:
	bool		_destroyed;
	bool		_stopped;
	float		_turningFactor;
	float		_currentTurningAngle;
	float		_accelerationFactor;	// How hard the engine is currently working
	float		_jumpCooldown;			// Cooldown when jump is activated
	float		_currentJumpCooldown;	// Current cooldown
	float		_engineTemperature;
	float		_engineCooldown;
	float		_engineOverload;
	float		_engineFlashTime;
	bool		_bEngineFlash;
	bool		_bEngineOverload;
	float		_velocity;				// Current forward velocity
	float		_timeSinceIntersection;	// Time since ray intersected track

	glm::vec3					_trackForward;			// Forward direction of track
	glm::vec3					_shipForward;			// Current forward direction of ship
	glm::vec3					_upDirection;			// Current up direction
	glm::vec3					_returnPos;				// Respawn position of ship

	SpringRotatedVector			_meshForwardDirection;	// Current facing direction
	SpringRotatedVector			_meshUpDirection;		// Up direction of ship mesh
	SpringRotatedVector			_cameraUpDirection;
	SpringRotatedVector			_cameraForwardDirection;

	SpringTranslatedVector		_meshPosition;			// Position of ship mesh in up direction
	SpringTranslatedVector		_meshXZPosition;		// Position of ship mesh in forward/right directions

	SurfaceType _currentSurface{ SurfaceType::normal };

	BoundingBox _boundingBox;

	glm::mat4	_meshMatrix;
	glm::mat4	_transformMatrix;

	const float _minAcceleration;
	const float _maxAcceleration;
	const float _maxTurningSpeed;
	const float _straighteningForce;		// How strongly the velocity direction turns toward track's forward direction
	const float _steerStraighteningForce;	// How strongly the ship resists turning
	const float _speedResistance;			// 'Air resistance'
	const float _preferredHeight;			// Desired ship height above track

	const float _rayHeight{ 5.0f };			// Height above ship of the origin of the rays used for intersection
	const float _rayAheadDistance{ 2.0f };	// Distance ahead of ship the second ray starts

	std::vector<SegmentInstance*> _segmentsToTest;	// Segments to test intersection against

	// Helper functions for update()
	void handleInputs(float dt);
	void handleCooldowns(float dt);
	void handleTemperature(float dt);
	void rotateTowardTrackForward(float dt);
	void updateDirectionsAndPositions(float dt);
	void trackSurface();
};
#endif // SHIP_HPP
