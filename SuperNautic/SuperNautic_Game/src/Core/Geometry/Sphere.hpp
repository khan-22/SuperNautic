#pragma once

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "glm/vec3.hpp"

struct Sphere
{
	glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);
    float radius = 0.f;

	Sphere(glm::vec3 center, float radius)
	: center(center)
	, radius(radius)
	{
	}

	Sphere()
	{
	}
};

#endif // SPHERE_HPP
