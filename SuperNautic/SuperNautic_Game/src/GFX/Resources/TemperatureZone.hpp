#pragma once

#ifndef TEMPERATURE_ZONE_HPP
#define TEMPERATURE_ZONE_HPP

#include "Core/Asset/LoadAssetFunctions.hpp"

#include <glm/glm.hpp>

namespace GFX
{
	struct TemperatureZone
	{
		ModelAsset				model;
		glm::mat4				modelMatrix;
		unsigned int			segmentIndex;
		std::vector<GLfloat>	temperatures;
	};
}

#endif // TEMPERATURE_ZONE_HPP