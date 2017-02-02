#pragma once

#ifndef RAYINTERSECTION_H
#define RAYINTERSECTION_H

#include "glm\vec3.hpp"

enum class SurfaceType : char { normal, hot, cold };

// Holds information about a detected collision
struct RayIntersection
{
	glm::vec3 position;
	glm::vec3 normal;
	SurfaceType surface;
};

#endif

