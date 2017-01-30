#include "ModelLoader.hpp"

#include <algorithm>

#include "../Core/LoadAssetFunctions.hpp"
#include "VertexArrayObject.hpp"

using namespace GFX;

ModelLoader::ModelLoader(/*const std::string& rootPath*/)
	//: _rootPath(rootPath)
{
}


ModelLoader::~ModelLoader()
{
}

Model* GFX::ModelLoader::loadModel(std::string filePath)
{
	RawMeshCollection* rawModel = RawMeshCache::get(filePath).get();
	if (rawModel == nullptr)
	{
		return nullptr;
	}

	// To minimize the amount of drawcalls we can group all
	// meshes with a similar texture into the same VertexBuffer.
	struct Grouping
	{
		std::vector<int> indices;
		int texture;
		GLsizei totalSizeInBytes;
	};
	std::vector<Grouping> groupings;

	// First we must group all raw meshes into groupings that
	// can be added to the same vertex buffer.
	for (int i = 0; i < rawModel->meshes.size(); i++)
	{
		RawVertexData& rawMesh = rawModel->meshes[i];

		GLsizei sizeOfMesh = 0;
		sizeOfMesh += rawMesh.vertices.size() * sizeof(rawMesh.vertices[0]);
		sizeOfMesh += rawMesh.texCoords.size() * sizeof(rawMesh.texCoords[0]);
		sizeOfMesh += rawMesh.normals.size() * sizeof(rawMesh.normals[0]);

		bool belongsToAGrouping = false;
		for (auto& group : groupings)
		{
			if (rawMesh.textureIndex == group.texture)
			{
				belongsToAGrouping = true;
				group.indices.push_back(i);
				group.totalSizeInBytes += sizeOfMesh;
			}
		}

		if (!belongsToAGrouping)
		{
			Grouping newGroup;
			newGroup.indices.push_back(i);
			newGroup.texture = rawMesh.textureIndex;
			newGroup.totalSizeInBytes = sizeOfMesh;
			groupings.push_back(newGroup);
		}
	}

	Model* model = new Model();
	for (auto& group : groupings)
	{
		VertexArrayObject& newMesh = model->addMesh().getVertexArrayObject();
		newMesh.addVertexBuffer(group.totalSizeInBytes, GL_STATIC_DRAW);
		

		// Send all vertex data...
		GLuint drawCount = 0U;
		GLuint offset = 0U;
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel->meshes[index];
			GLsizei sizeInBytes = rawMesh.vertices.size() * sizeof(rawMesh.vertices[0]);
			newMesh.sendDataToBuffer(0, 0, offset, sizeInBytes, rawMesh.vertices.data(), 3, GL_FLOAT);
			offset += sizeInBytes;

			drawCount += rawMesh.vertices.size();
		}
		newMesh.setDrawCount(drawCount);

		// Send all texCoord data...
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel->meshes[index];
			GLsizei sizeInBytes = rawMesh.texCoords.size() * sizeof(rawMesh.texCoords[0]);
			newMesh.sendDataToBuffer(0, 1, offset, sizeInBytes, rawMesh.texCoords.data(), 3, GL_FLOAT);
			offset += sizeInBytes;
		}

		// Send all normal data...
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel->meshes[index];
			GLsizei sizeInBytes = rawMesh.normals.size() * sizeof(rawMesh.normals[0]);
			newMesh.sendDataToBuffer(0, 2, offset, sizeInBytes, rawMesh.normals.data(), 3, GL_FLOAT);
			offset += sizeInBytes;
		}
	}
	
	return model;
}


