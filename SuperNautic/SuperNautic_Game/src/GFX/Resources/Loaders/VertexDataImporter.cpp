#include "GFX/Resources/Loaders/VertexDataImporter.hpp"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

#include "Core/Io/Log.hpp"

using namespace GFX;

struct Header
{
public:
	uint32_t magicNumber = 149;
	uint32_t numMeshes = 0;
	uint32_t numCameras = 0;
	uint32_t numLights = 0;
};

struct MeshHeader
{
	uint32_t numVertices = 0;
	uint32_t numFaces = 0;
	uint32_t nameLength = 0;
};

VertexDataImporter::VertexDataImporter(const std::string& rootPath)
	: _rootPath(rootPath)
{
}


VertexDataImporter::~VertexDataImporter()
{
}

RawMeshCollection* VertexDataImporter::importVertexData(std::string filepath)
{


	filepath = _rootPath + filepath;

	LOG("Started loading file: " + filepath);

	if (filepath.find(".fbx", 0) != std::string::npos)
	{
		LOG(">>> STOP! NO FBX ALLOWED!");
		return nullptr;
	}
	else if (filepath.find(".blend", 0) != std::string::npos)
	{
		LOG(">>> STOP! NO BLEND ALLOWED!");
		return nullptr;
	}

	RawMeshCollection* collection = nullptr;

//	FILE* in = nullptr;

	std::ifstream in(filepath, std::ios::binary);


//	fopen_s(&in, filepath.c_str(), "rb");

    if(!in.good())
//	if(in == nullptr)
	{
		LOG(">>> Failed to find file!");
		return nullptr;
	}

	collection = new RawMeshCollection();

	Header header;

	in.read((char*)&header, sizeof(Header));
//	fread_s(&header, sizeof(Header), sizeof(Header), 1, in);

	for (int i = 0; i < header.numMeshes; i++)
	{
		collection->meshes.emplace_back();
		RawVertexData& currentMesh = collection->meshes.back();

		MeshHeader meshHeader;
		in.read((char*)&meshHeader, sizeof(MeshHeader));
//		fread_s(&meshHeader, sizeof(MeshHeader), sizeof(MeshHeader), 1, in);

		currentMesh.name.resize(meshHeader.nameLength);
		currentMesh.vertices.resize(meshHeader.numVertices);
		currentMesh.texCoords.resize(meshHeader.numVertices);
		currentMesh.normals.resize(meshHeader.numVertices);
		currentMesh.faces.resize(meshHeader.numFaces);
		currentMesh.indices.resize(meshHeader.numFaces * 3);

		in.read((char*)&currentMesh.name[0], currentMesh.name.size() * sizeof(currentMesh.name[0]));
		in.read((char*)&currentMesh.vertices[0], currentMesh.vertices.size() * sizeof(currentMesh.vertices[0]));
		in.read((char*)&currentMesh.texCoords[0], currentMesh.texCoords.size() * sizeof(currentMesh.texCoords[0]));
		in.read((char*)&currentMesh.normals[0], currentMesh.normals.size() * sizeof(currentMesh.normals[0]));
		in.read((char*)&currentMesh.faces[0], currentMesh.faces.size() * sizeof(currentMesh.faces[0]));
//		fread_s(&currentMesh.name[0], currentMesh.name.size() * sizeof(currentMesh.name[0]), sizeof(currentMesh.name[0]), currentMesh.name.size(), in);
//		fread_s(&currentMesh.vertices[0], currentMesh.vertices.size() * sizeof(currentMesh.vertices[0]), sizeof(currentMesh.vertices[0]), currentMesh.vertices.size(), in);
//		fread_s(&currentMesh.texCoords[0], currentMesh.texCoords.size() * sizeof(currentMesh.texCoords[0]), sizeof(currentMesh.texCoords[0]), currentMesh.texCoords.size(), in);
//		fread_s(&currentMesh.normals[0], currentMesh.normals.size() * sizeof(currentMesh.normals[0]), sizeof(currentMesh.normals[0]), currentMesh.normals.size(), in);
//		fread_s(&currentMesh.faces[0], currentMesh.faces.size() * sizeof(currentMesh.faces[0]), sizeof(currentMesh.faces[0]), currentMesh.faces.size(), in);

		// Go back to read index data again
		in.seekg(-(int)(currentMesh.faces.size() * sizeof(currentMesh.faces[0])), in.cur);
//		fseek(in, -(int)(currentMesh.faces.size() * sizeof(currentMesh.faces[0])), SEEK_CUR);

        in.read((char*)&currentMesh.indices[0], currentMesh.indices.size() * sizeof(currentMesh.indices[0]));
//		fread_s(&currentMesh.indices[0], currentMesh.indices.size() * sizeof(currentMesh.indices[0]), sizeof(currentMesh.indices[0]), currentMesh.indices.size(), in);
	}

	for (int i = 0; i < header.numCameras; i++)
	{
		collection->cameras.emplace_back();
		glm::mat4& cameraMat = collection->cameras.back();

		in.read((char*)&cameraMat[0][0], sizeof(cameraMat));
//		fread_s(&cameraMat[0][0], sizeof(cameraMat), sizeof(cameraMat), 1, in);
	}

	in.close();
//	fclose(in);

	LOG("Finished loading: ", filepath);

	return collection;
}


