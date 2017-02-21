#pragma once

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <array>

#include "glm/vec3.hpp"

// Bounding box enclosing a segment
struct BoundingBox
{
	glm::vec3				center;

	// x, y, z directions
	std::array<glm::vec3, 3>	directions;

	// x, y, z halflengths
	std::array<float, 3>	halfLengths;

	BoundingBox(const glm::vec3& center, const std::array<glm::vec3, 3>& directions, const std::array<float, 3>& halfLengths)
	: center(center)
	, directions(directions)
	, halfLengths(halfLengths)
	{
	}

	BoundingBox() : center{ glm::vec3{} },
		halfLengths{ -1.0f, -1.0f, -1.0f }
	{ }
};

#endif
