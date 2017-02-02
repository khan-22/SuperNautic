#pragma once

#include "AssetCache.hpp"

#include "../GFX/VertexDataImporter.hpp"
#include "../GFX/ShaderLoader.hpp"
#include "../GFX/ModelLoader.hpp"
#include "../GFX/Texture.hpp"

using RawMeshCache	= AssetCache<GFX::RawMeshCollection, std::string>;
using ShaderCache	= AssetCache<GFX::Shader, std::string>;
using ModelCache	= AssetCache<GFX::Model, std::string>;
using TextureCache  = AssetCache<GFX::Texture, std::string>;

using RawMeshAsset	= Asset<GFX::RawMeshCollection>;
using ShaderAsset	= Asset<GFX::Shader>;
using ModelAsset	= Asset<GFX::Model>;
using TextureAsset  = Asset<GFX::Texture>;
