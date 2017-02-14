#pragma once

#ifndef MATERIAL_LOADER_HPP
#define MATERIAL_LOADER_HPP

#include "Material.hpp"

namespace GFX
{

	class MaterialLoader
	{
    public:
        static std::shared_ptr<Material> load(const std::string& filePath);
	};
}

#endif //MATERIAL_LOADER_HPP
