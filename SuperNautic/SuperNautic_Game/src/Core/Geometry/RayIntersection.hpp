#pragma once

#ifndef RAYINTERSECTION_H
#define RAYINTERSECTION_H

#include "glm\vec3.hpp"

// Holds information about a detected collision
struct RayIntersection
{
	glm::vec3	_position;
	glm::vec3	_normal;
	float _surface;
	bool		_hit{ false };
	float		_length;

	RayIntersection(bool hit)
	{
		_hit = hit;
	}

	RayIntersection(bool hit, glm::vec3 position, glm::vec3 normal, float surface, float length)
	{
		_hit = hit;
		_position = position;
		_normal = normal;
		_surface = surface;
		_length = length;
	}

	operator bool() { return _hit; }
};

#endif

