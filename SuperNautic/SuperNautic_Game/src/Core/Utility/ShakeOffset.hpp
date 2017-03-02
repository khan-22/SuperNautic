#pragma once

#ifndef SHAKEOFFSET_H
#define SHAKEOFFSET_H

#include "glm/vec3.hpp"

class ShakeOffset
{
public:
	ShakeOffset(float horizontalSpeed, float horizontalMagnitude, float verticalSpeed, float verticalMagnitude, float decaySpeed, float baseShakeSpeed);

	void update(float dt);

	glm::vec3 getVector() const;
	glm::vec3 operator()() const;

	void setMagnitude(float shakeMagnitude);
	void setSpeed(float shakeSpeed);
	void setDecaySpeed(float decaySpeed);
private:
	const float _horizontalSpeed;
	const float _horizontalMagnitude;

	const float _verticalSpeed;
	const float _verticalMagnitude;

	float _hAccumulator;	// Used in position calculation
	float _vAccumulator;	// Used in position calculation
	float _shakeMagnitude;	// Current shake magnitude
	float _shakeSpeed;		// Current shake speed
	float _decaySpeed;		// Speed of _shakeMagnitude and _shakeSpeed decay

	const float _baseSpeed;		// Lowest shake speed
};

#endif