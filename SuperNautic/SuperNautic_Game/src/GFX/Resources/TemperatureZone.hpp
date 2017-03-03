#pragma once

#ifndef TEMPERATURE_ZONE_HPP
#define TEMPERATURE_ZONE_HPP

#include "GFX/Resources/Model.hpp"

#include <glm/glm.hpp>

namespace GFX
{
	struct TemperatureZone
	{
		Model		model;
		glm::mat4	modelMatrix;
		float		temperatures[4];

	};
}

#endif // TEMPERATURE_ZONE_HPP