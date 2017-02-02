#pragma once

#ifndef RAY_H
#define RAY_H

#include "glm\vec3.hpp"

// A ray used for intersection testing
class Ray
{
private:
	glm::vec3	_origin;
	glm::vec3	_direction;

	// 1/x, 1/y, 1/z
	// Used to avoid calculating in every intersection test
	glm::vec3	_invDirection;

	float		_length;

public:
	Ray(glm::vec3 origin, glm::vec3 direction, float length) : _origin{ origin }, _direction{ glm::normalize(direction) }, _length{ length }
	{
		_invDirection = { 1.0f / _direction.x, 1.0f / _direction.y, 1.0f / _direction.z };
	}

	const glm::vec3& origin() const { return _origin; }
	const glm::vec3& direction() const { return _direction; }
	const glm::vec3& invDirection() const { return _invDirection; }
	float length() const { return _length; }

	void origin(const glm::vec3& origin) { _origin = origin; }

	void direction(const glm::vec3& direction) 
	{ 
		_direction = glm::normalize(direction);
		_invDirection = { 1.0f / _direction.x, 1.0f / _direction.y, 1.0f / _direction.z }; 
	}

	void length(float length) { _length = length; }
};

#endif