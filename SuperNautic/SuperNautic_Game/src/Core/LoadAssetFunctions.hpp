#pragma once

#ifndef LOAD_ASSET_FUNCTIONS_HPP
#define LOAD_ASSET_FUNCTIONS_HPP

#include "AssetCache.hpp"

#include "../GFX/VertexDataImporter.hpp"
#include "../GFX/ShaderLoader.hpp"
#include "../GFX/ModelLoader.hpp"
#include "../GFX/Texture.hpp"
#include "../GFX/Material.hpp"

using RawMeshCache	= AssetCache<GFX::RawMeshCollection, std::string>;
using ShaderCache	= AssetCache<GFX::Shader, std::string>;
using ModelCache	= AssetCache<GFX::Model, std::string>;
using TextureCache  = AssetCache<GFX::Texture, std::string>;
using MaterialCache = AssetCache<GFX::Material, std::string>;

using RawMeshAsset	= Asset<GFX::RawMeshCollection>;
using ShaderAsset	= Asset<GFX::Shader>;
using ModelAsset	= Asset<GFX::Model>;
using TextureAsset  = Asset<GFX::Texture>;
using MaterialAsset = Asset<GFX::Material>;

#endif // LOAD_ASSET_FUNCTIONS_HPP
