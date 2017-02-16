#include "GFX/Resources/Loaders/VertexDataImporter.hpp"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

#include "Core/Io/Log.hpp"

namespace GFX {

VertexDataImporter::VertexDataImporter(const std::string& rootPath)
	: _rootPath(rootPath)
{
}


VertexDataImporter::~VertexDataImporter()
{
}

RawMeshCollection* VertexDataImporter::importVertexData(std::string filepath)
{
	RawMeshCollection* collection = nullptr;

	filepath = _rootPath + filepath;

	LOG("Started loading file: " + filepath);




	LOG("Loaded ", collection->meshes.size(), " meshes...");

	return collection;
}

}
