#pragma once

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <vector>

#include "Core/Asset/Asset.hpp"
#include "GFX/Resources/Texture.hpp"

namespace GFX
{
	class Material
	{
    public:
	    using TextureAsset = Asset<GFX::Texture>;
        struct Group
        {
            Group(TextureAsset& diffuse, TextureAsset& specular, TextureAsset& normal, TextureAsset& illumination);

            TextureAsset diffuse;
            TextureAsset specular;
			TextureAsset normal;
			TextureAsset illumination;
        };

        Material(const std::vector<Group>& groups);

        void bind(unsigned int groupId) const;
        void unbind(unsigned int groupId) const;

        size_t getNumGroups() const;

    private:
        std::vector<Group> _groups;
	};
}

#endif //MATERIAL_HPP
