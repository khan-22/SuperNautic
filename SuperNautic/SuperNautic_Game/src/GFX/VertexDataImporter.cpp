#include "VertexDataImporter.h"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "../Log.h"

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

	const aiScene* importedData = aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (importedData == nullptr)
	{
		LOG_ERROR("Failed to load mesh: ", filepath);
		return nullptr;
	}

	collection = new RawMeshCollection();
	for (int i = 0; i < importedData->mNumMeshes; i++)
	{
		aiMesh* mesh = importedData->mMeshes[i];

		collection->meshes.emplace_back();
		RawVertexData& data = collection->meshes.back();

		data.vertices.resize(mesh->mNumVertices);
		data.texCoords.resize(mesh->mNumVertices);
		data.normals.resize(mesh->mNumVertices);

		memcpy(data.vertices.data(), &mesh->mVertices[0], sizeof(mesh->mVertices[0]) * mesh->mNumVertices);
		memcpy(data.texCoords.data(), &mesh->mTextureCoords[0], sizeof(mesh->mTextureCoords[0]) * mesh->mNumVertices);
		memcpy(data.normals.data(), &mesh->mNormals[0], sizeof(mesh->mNormals[0]) * mesh->mNumVertices);

		data.name = mesh->mName.C_Str();
	}

	LOG("Loaded ", collection->meshes.size(), " meshes...");

	return collection;
}

}
