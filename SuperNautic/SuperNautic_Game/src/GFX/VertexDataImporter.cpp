#include "VertexDataImporter.hpp"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include "../Log.hpp"

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
		return nullptr;
	}

	collection = new RawMeshCollection();

	// Import meshes
	for (int i = 0; i < importedData->mNumMeshes; i++)
	{
		aiMesh* mesh = importedData->mMeshes[i];

		collection->meshes.emplace_back();
		RawVertexData& data = collection->meshes.back();

		data.vertices.resize(mesh->mNumVertices);
		data.texCoords.resize(mesh->mNumVertices);
		data.normals.resize(mesh->mNumVertices);
		data.faces.resize(mesh->mNumFaces);
		data.indices.resize(mesh->mNumFaces * 3);

		memcpy(&data.vertices[0], &mesh->mVertices[0], sizeof(mesh->mVertices[0]) * mesh->mNumVertices);
		memcpy(&data.texCoords[0], &mesh->mTextureCoords[0], sizeof(mesh->mTextureCoords[0]) * mesh->mNumVertices);
		memcpy(&data.normals[0], &mesh->mNormals[0], sizeof(mesh->mNormals[0]) * mesh->mNumVertices);
		//memcpy(&data.faces[0], &mesh->mFaces[0], sizeof(mesh->mFaces[0].mIndices[0]) * 3 * mesh->mNumFaces);

		// Faces must be copied manually
		data.largestIndex = 0U;
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace currentFace = mesh->mFaces[i];
			data.faces[i] = glm::uvec3(currentFace.mIndices[0], currentFace.mIndices[1], currentFace.mIndices[2]);
			data.indices.push_back(data.faces[i].x);
			data.indices.push_back(data.faces[i].y);
			data.indices.push_back(data.faces[i].z);
			GLuint largest = std::max(data.faces[i].x, std::max(data.faces[i].y, data.faces[i].z));
			if (largest > data.largestIndex)
			{
				data.largestIndex = largest;
			}
		}

		data.textureIndex = mesh->mMaterialIndex;
		
		data.name = mesh->mName.C_Str();
	}

	// Import cameras...
	for (int i = 0; i < importedData->mNumCameras; i++)
	{
		glm::mat4 endMatrix;
		aiMatrix4x4 cameraMatrix;
		importedData->mCameras[i]->GetCameraMatrix(cameraMatrix);
		
		endMatrix = { cameraMatrix.a1, cameraMatrix.a2, cameraMatrix.a3, cameraMatrix.a4,
					  cameraMatrix.b1, cameraMatrix.b2, cameraMatrix.b3, cameraMatrix.b4,
					  cameraMatrix.c1, cameraMatrix.c2, cameraMatrix.c3, cameraMatrix.c4,
					  cameraMatrix.d1, cameraMatrix.d2, cameraMatrix.d3, cameraMatrix.d4 };

		collection->cameras.push_back(endMatrix);
	}

	LOG("Loaded ", collection->meshes.size(), " meshes...");

	return collection;
}

}
