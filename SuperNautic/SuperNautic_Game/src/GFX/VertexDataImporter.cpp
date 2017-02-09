#include "VertexDataImporter.hpp"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

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

	LOG("Started loading file: " + filepath);

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

		data.indices.reserve(mesh->mNumFaces * 3);

		memcpy(&data.vertices[0], &mesh->mVertices[0], sizeof(mesh->mVertices[0]) * mesh->mNumVertices);
		memcpy(&data.texCoords[0], &mesh->mTextureCoords[0][0], sizeof(mesh->mTextureCoords[0][0]) * mesh->mNumVertices);
		memcpy(&data.normals[0], &mesh->mNormals[0], sizeof(mesh->mNormals[0]) * mesh->mNumVertices);
		//memcpy(&data.faces[0], &mesh->mFaces[0], sizeof(mesh->mFaces[0].mIndices[0]) * 3 * mesh->mNumFaces);

		// Faces must be copied manually
		data.largestIndex = 0U;
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace currentFace = mesh->mFaces[i];
			if (currentFace.mNumIndices != 3)
			{
				LOG_ERROR("Found a face with != 3 indices! This is not allowed.");
			}
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
		aiMatrix4x4 cameraMatrix;

		//importedData->mCameras[i]->
		aiCamera* cam = importedData->mCameras[i];
		glm::vec4 pos({ cam->mPosition.x, cam->mPosition.y, cam->mPosition.z, 1.f });
		glm::vec4 dir({ cam->mLookAt.x, cam->mLookAt.y, cam->mLookAt.z, 1.f });
		glm::vec4 up({ cam->mUp.x, cam->mUp.y, cam->mUp.z, 0.f });

		aiNode* rootNode = importedData->mRootNode;
		aiNode* cameraNode = rootNode->FindNode(cam->mName);
		aiMatrix4x4 aiCamTrans = cameraNode->mTransformation;

		glm::mat4 transMat = { aiCamTrans.a1, aiCamTrans.b1, aiCamTrans.c1, aiCamTrans.d1,
							aiCamTrans.a2, aiCamTrans.b2, aiCamTrans.c2, aiCamTrans.d2,
							aiCamTrans.a3, aiCamTrans.b3, aiCamTrans.c3, aiCamTrans.d3,
							aiCamTrans.a4, aiCamTrans.b4, aiCamTrans.c4, aiCamTrans.d4 };

		pos = transMat * pos;
		dir = transMat * dir;
		up = transMat * up;

		glm::vec3 realPos = glm::vec3(pos.x, pos.z, -pos.y);
		glm::vec3 realDir = glm::vec3(dir.x, dir.z, -dir.y);
		glm::vec3 realUp = glm::vec3(up.x, up.z, -up.y);

		glm::mat4 endMatrix = glm::inverse(glm::lookAt(realPos - glm::normalize(realDir) * 0.1f, realDir, realUp));


		collection->cameras.push_back(endMatrix);
	}

	LOG("Loaded ", collection->meshes.size(), " meshes...");

	return collection;
}

}
