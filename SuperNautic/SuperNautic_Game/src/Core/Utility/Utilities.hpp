#pragma once

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Core/Io/Log.hpp"
#include "glm/vec3.hpp"

template<typename T>
inline T clamp(T value, T low, T high)
{
	if (low > high)
	{
		LOG_ERROR("Clamp error: low > high");
		return value;
	}

	if (value < low)
	{
		return low;
	}

	if (value > high)
	{
		return high;
	}

	return value;
}

bool floatEq(float a, float b);

// True if two vectors are almost equal
inline bool bAlmostEqual(glm::vec3 vector1, glm::vec3 vector2)
{
	return (0.0001f > abs(vector1.x - vector2.x)) &&
		(0.0001f > abs(vector1.y - vector2.y)) &&
		(0.0001f > abs(vector1.z - vector2.z));
}

#endif
