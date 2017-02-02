#pragma once

#ifndef AABB_H
#define AABB_H

#include <vector>

#include "glm\vec3.hpp"

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

	// Test if a ray intersects this box, returns intersection position along ray
	

	AABB(glm::vec3 min, glm::vec3 max)
		: _minCorner{ min }, _maxCorner{ max }
	{ }

	AABB()
	{ }
};

#endif