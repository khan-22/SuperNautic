#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Core/Asset/LoadAssetFunctions.hpp"

namespace GFX
{
	struct Window
	{
		ModelAsset		windowModel;
		unsigned int	segmentIndex;
		glm::mat4		modelTransform;
	};
}

#endif //WINDOW_HPP