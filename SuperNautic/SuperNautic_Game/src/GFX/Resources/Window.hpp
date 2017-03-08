#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Core/Asset/LoadAssetFunctions.hpp"

namespace GFX
{
	struct Window
	{
		ModelAsset		windowModel;
		glm::mat4		modelTransform;
		unsigned int 	segmentIndex;
	};
}

#endif //WINDOW_HPP