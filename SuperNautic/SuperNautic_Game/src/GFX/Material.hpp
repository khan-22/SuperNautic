#pragma once

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <vector>

#include "../Core/Asset.hpp"
#include "Texture.hpp"

namespace GFX
{
	class Material
	{
    public:
	    using TextureAsset = Asset<GFX::Texture>;
        struct Group
        {
            Group(TextureAsset& diffuse, TextureAsset& specular, TextureAsset& normal);

            TextureAsset diffuse;
            TextureAsset specular;
            TextureAsset normal;
        };

        Material(const std::vector<Group>& groups);

        void bind(unsigned int groupId) const;
        void unbind(unsigned int groupId) const;

    private:
        std::vector<Group> _groups;
	};
}

#endif //MATERIAL_HPP
