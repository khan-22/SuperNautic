#pragma once

#ifndef LOAD_ASSET_FUNCTIONS_HPP
#define LOAD_ASSET_FUNCTIONS_HPP


#include "SFML/Graphics/Font.hpp"

#include "Core/Asset/AssetCache.hpp"
#include "GFX/Resources/Loaders/VertexDataImporter.hpp"
#include "GFX/Resources/Loaders/ShaderLoader.hpp"
#include "GFX/Resources/Loaders/ModelLoader.hpp"
#include "GFX/Resources/Texture.hpp"
#include "GFX/Resources/Material.hpp"

using RawMeshCache		= AssetCache<GFX::RawMeshCollection, std::string>;
using ShaderCache		= AssetCache<GFX::Shader, std::string>;
using ModelCache		= AssetCache<GFX::Model, std::string>;
using TextureCache		= AssetCache<GFX::Texture, std::string>;
using MaterialCache		= AssetCache<GFX::Material, std::string>;
using FontCache			= AssetCache<sf::Font, std::string>;
using SFMLTextureCache	= AssetCache<sf::Texture, std::string>;

using RawMeshAsset		= Asset<GFX::RawMeshCollection>;
using ShaderAsset		= Asset<GFX::Shader>;
using ModelAsset		= Asset<GFX::Model>;
using TextureAsset		= Asset<GFX::Texture>;
using MaterialAsset		= Asset<GFX::Material>;
using FontAsset			= Asset<sf::Font>;
using SFMLTextureAsset	= Asset<sf::Texture>;

#endif // LOAD_ASSET_FUNCTIONS_HPP
