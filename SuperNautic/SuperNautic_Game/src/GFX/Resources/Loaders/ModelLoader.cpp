#include "GFX/Resources/Loaders/ModelLoader.hpp"

#include <algorithm>

#include "Core/Asset/LoadAssetFunctions.hpp"
#include "GFX/Resources/VertexArrayObject.hpp"

using namespace GFX;

// To minimize the amount of drawcalls we can group all
// meshes with a similar texture into the same VertexBuffer.
struct ModelLoader::Grouping
{
	std::vector<int> indices;
	int texture;
	GLsizei totalSizeInBytes;		// Size of all vertex-data in bytes
	GLsizei totalIndexSizeInBytes;	// Size of all index-data in bytes
};

ModelLoader::ModelLoader(/*const std::string& rootPath*/)
	//: _rootPath(rootPath)
{
}


ModelLoader::~ModelLoader()
{
}

Model* GFX::ModelLoader::loadModel(std::string filePath)
{
	RawMeshAsset rawModel = RawMeshCache::get(filePath);
	
	if (rawModel.get() == nullptr)
	{
		return nullptr;
	}

	LOG("Beginning processing on model: ", filePath);

	std::vector<ModelLoader::Grouping> groupings = generateGroupings(rawModel.get());

	LOG("Model was grouped into ", groupings.size(), " group(s).");

	Model* model = new Model();
	for (auto& group : groupings)
	{
		VertexArrayObject& newMesh = model->addMesh().getVertexArrayObject();
		newMesh.addVertexBuffer(group.totalSizeInBytes, GL_STATIC_DRAW);
		newMesh.addIndexBuffer(group.totalIndexSizeInBytes, GL_STATIC_DRAW);

		// Send all index data...
		GLuint drawCount = 0U;
		GLuint byteIndexOffset = 0U;
		GLuint indexOffset = 0U;
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel.get()->meshes[index];
			std::vector<GLuint> indices = rawMesh.indices;
			if (indexOffset != 0U)
			{
				for (GLuint& index : indices)
				{
					index += indexOffset;
				}

			}
			GLsizei sizeInBytesIndices = indices.size() * sizeof(rawMesh.indices[0]);
			newMesh.sendDataToIndexBuffer(byteIndexOffset, sizeInBytesIndices, indices.data());
			byteIndexOffset += sizeInBytesIndices;
			indexOffset += rawMesh.largestIndex + 1;

			LOG("Sent ", indices.size(), " indices to index buffer.");

			drawCount += indices.size();
		}
		newMesh.setDrawCount(drawCount);

		// Send all vertex position data...
		GLuint byteDataOffset = 0U;
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel.get()->meshes[index];
			GLsizei sizeInBytes = rawMesh.vertices.size() * sizeof(rawMesh.vertices[0]);
			newMesh.sendDataToBuffer(0, 0, byteDataOffset, sizeInBytes, rawMesh.vertices.data(), 3, GL_FLOAT);
			byteDataOffset += sizeInBytes;

			LOG("Sent ", rawMesh.vertices.size(), " vertices to vertex buffer.");
		}

		// Send all texCoord data...
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel.get()->meshes[index];
			GLsizei sizeInBytes = rawMesh.texCoords.size() * sizeof(rawMesh.texCoords[0]);
			newMesh.sendDataToBuffer(0, 1, byteDataOffset, sizeInBytes, rawMesh.texCoords.data(), 3, GL_FLOAT);
			byteDataOffset += sizeInBytes;

			LOG("Sent ", rawMesh.texCoords.size(), " texCoords to vertex buffer.");
		}

		// Send all normal data...
		for (int index : group.indices)
		{
			RawVertexData& rawMesh = rawModel.get()->meshes[index];
			GLsizei sizeInBytes = rawMesh.normals.size() * sizeof(rawMesh.normals[0]);
			newMesh.sendDataToBuffer(0, 2, byteDataOffset, sizeInBytes, rawMesh.normals.data(), 3, GL_FLOAT);
			byteDataOffset += sizeInBytes;

			LOG("Sent ", rawMesh.normals.size(), " normals to vertex buffer.");
		}
	}
	
	return model;
}

std::vector<ModelLoader::Grouping> GFX::ModelLoader::generateGroupings(RawMeshCollection * rawModel)
{
	std::vector<ModelLoader::Grouping> groupings;
	// First we must group all raw meshes into groupings that
	// can be added to the same vertex buffer.
	for (int i = 0; i < rawModel->meshes.size(); i++)
	{
		RawVertexData& rawMesh = rawModel->meshes[i];

		GLsizei sizeOfMesh = 0;
		sizeOfMesh += rawMesh.vertices.size() * sizeof(rawMesh.vertices[0]);
		sizeOfMesh += rawMesh.texCoords.size() * sizeof(rawMesh.texCoords[0]);
		sizeOfMesh += rawMesh.normals.size() * sizeof(rawMesh.normals[0]);

		GLsizei sizeOfIndices = 0;
		sizeOfIndices += rawMesh.indices.size() * sizeof(rawMesh.indices[0]);

		bool belongsToAGrouping = false;
		for (auto& group : groupings)
		{
			if (rawMesh.textureIndex == group.texture)
			{
				belongsToAGrouping = true;
				group.indices.push_back(i);
				group.totalSizeInBytes += sizeOfMesh;
				group.totalIndexSizeInBytes += sizeOfIndices;
			}
		}

		if (!belongsToAGrouping)
		{
			Grouping newGroup;
			newGroup.indices.push_back(i);
			newGroup.texture = rawMesh.textureIndex;
			newGroup.totalSizeInBytes = sizeOfMesh;
			newGroup.totalIndexSizeInBytes = sizeOfIndices;
			groupings.push_back(newGroup);
		}
	}

	return groupings;
}


