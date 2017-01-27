// EXAMPLE THING.
// TODO: Talk Karl/Toby through about this.

#include "LoadAsset.hpp"

#include "../GFX/VertexDataImporter.h"
#include "../GFX/ShaderLoader.h"

struct Mesh
{

};

template<>
std::shared_ptr<GFX::RawMeshCollection> loadAsset<GFX::RawMeshCollection>(std::string key)
{
	GFX::VertexDataImporter importer("./res/models/");
    return std::shared_ptr<GFX::RawMeshCollection>(importer.importVertexData(key));
}

template<>
std::shared_ptr<GFX::Shader> loadAsset<GFX::Shader>(std::string key)
{
	GFX::ShaderLoader loader("./src/GFX/Shaders/");
	return std::shared_ptr<GFX::Shader>(loader.loadShader(key));
}
