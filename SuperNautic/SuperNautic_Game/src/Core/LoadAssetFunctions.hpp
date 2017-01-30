#pragma once

#include "AssetCache.hpp"

#include "../GFX/VertexDataImporter.hpp"
#include "../GFX/ShaderLoader.hpp"
#include "../GFX/ModelLoader.hpp"

using RawMeshCache	= AssetCache<GFX::RawMeshCollection, std::string>;
using ShaderCache	= AssetCache<GFX::Shader, std::string>;
using ModelCache	= AssetCache<GFX::Model, std::string>;

using RawMeshAsset	= Asset<GFX::RawMeshCollection>;
using ShaderAsset	= Asset<GFX::Shader>;
using ModelAsset	= Asset<GFX::Model>;
