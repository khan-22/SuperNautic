#pragma once

#ifndef UTILITIES_H
#define UTILITIES_H

#include "../../Log.hpp"

template<typename T>
inline T& clamp(T& value, const T& low, const T& high)
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

#endif