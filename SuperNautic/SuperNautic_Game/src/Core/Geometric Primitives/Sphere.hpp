#pragma once

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "glm/vec3.hpp"

struct Sphere
{
	glm::vec3 center;
    float radius;

	Sphere(glm::vec3 center, float radius)
	: center(center)
	, radius(radius)
	{
	}
};

#endif // SPHERE_HPP
