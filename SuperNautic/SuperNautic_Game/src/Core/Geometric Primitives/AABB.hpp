#pragma once

#ifndef AABB_H
#define AABB_H

#include <vector>

#include "glm\vec3.hpp"
#include "Ray.hpp"

// Axis aligned bounding box used for oct-tree
struct AABB
{
	// The children of this box
	std::vector<AABB>					_children;

	// Will contain indices into face vectors of models
	// [0]..[_temperatureZoneCollisions.size()] is zone collisions, [_temperatureZoneCollisions.size()] is base collision 
	std::vector<std::vector<unsigned>> faceIndices;

	// Positions of max and min corners of this box
	glm::vec3							_minCorner;
	glm::vec3							_maxCorner;

	// Test if a ray intersects this box, returns intersection position along ray, -1.0 if no hit
	float rayIntersection(const Ray& ray)
	{
		float tmin = 0;
		float tmax = ray.length();

		for (unsigned i = 0; i < 3; ++i)
		{
			float t0 = (_minCorner[i] - ray.origin()[i]) * ray.invDirection()[i];
			float t1 = (_maxCorner[i] - ray.origin()[i]) * ray.invDirection()[i];

			if (ray.invDirection()[i] < 0.0f)
			{
				std::swap(t0, t1);
			}

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if (tmax <= tmin)
				return -1.0f;
		}
		return (tmin < ray.length()) ? tmin : -1.0f;
	}

	AABB(glm::vec3 min, glm::vec3 max)
		: _minCorner{ min }, _maxCorner{ max }
	{ }

	AABB()
	{ }
};

#endif