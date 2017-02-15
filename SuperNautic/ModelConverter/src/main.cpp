#include <iostream>

#include <Windows.h>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <vector>

#define HEADER_RESERVED 128
#define HEADER_RESERVED_BYTES sizeof(uint32_t) * HEADER_RESERVED

// Globals
HANDLE gConsoleHandle;
CONSOLE_SCREEN_BUFFER_INFO gPreviousState;

enum LogColor : WORD
{
	BLUE	= 0x1 | 0x8,
	GREEN	= 0x2 | 0x8,
	RED		= 0x4 | 0x8,
	
	CYAN	= BLUE  | GREEN,
	PURPLE	= BLUE  | RED,
	YELLOW	= GREEN | RED,

	WHITE = BLUE | GREEN | RED,
};

std::ostream& log(LogColor color)
{
	SetConsoleTextAttribute(gConsoleHandle, color);
	return std::cout;
}

char* getNameOfFile(char* fullPath)
{
	char* place = fullPath;
	char* lastPlace = place;
	while (*place != '\0')
	{
		if (*place == '\\')
		{
			lastPlace = place;
		}
		place++;
	}

	return lastPlace;
}

std::string getPathExceptEnding(char* fullPath)
{
	std::string editedPath(fullPath);
	editedPath = editedPath.substr(0, editedPath.find_last_of('.'));

	return editedPath;
}

glm::vec3 toGLM(aiVector3D vec)
{
	return{ vec.x, vec.y, vec.z };
}

void processPositions(std::vector<glm::vec3>& positions, const aiMesh* importedMesh)
{
	log(YELLOW) << "Positions: 0%";
	positions.reserve(positions.capacity() + importedMesh->mNumVertices);
	for (int i = 0; i < importedMesh->mNumVertices; i++)
	{
		positions.push_back(toGLM(importedMesh->mVertices[i]));

		if (i >= importedMesh->mNumVertices / 2)
		{
			log(YELLOW) << "\rPositions: 50%";
		}
	}
	log(GREEN) << "\rPositions: 100%" << std::endl;
}

void processTexCoords(std::vector<glm::vec3>& texCoords, const aiMesh* importedMesh)
{
	texCoords.reserve(texCoords.capacity() + importedMesh->mNumVertices);
	if (importedMesh->mTextureCoords[0] != nullptr)
	{
		log(YELLOW) << "TexCoord: 0%";
		for (int i = 0; i < importedMesh->mNumVertices; i++)
		{
			glm::vec3 uvw = toGLM(importedMesh->mTextureCoords[0][i]);
			uvw.z = (float)(importedMesh->mMaterialIndex) + 0.1f;
			texCoords.push_back(uvw);

			if (i >= importedMesh->mNumVertices / 2)
			{
				log(YELLOW) << "\rTexCoord: 50%";
			}
		}
	}
	else
	{
		log(YELLOW) << "The mesh should have texture coordinates. All are set to 0." << std::endl;
		for (int i = 0; i < importedMesh->mNumVertices; i++)
		{
			texCoords.push_back({ 0.f, 0.f, 0.f });
		}
	}
	log(GREEN) << "\rTexCoord: 100%" << std::endl;
}

void processNormals(std::vector<glm::vec3>& normals, const aiMesh* importedMesh)
{
	log(YELLOW) << "Normals: 0%";
	normals.reserve(normals.capacity() + importedMesh->mNumVertices);
	for (int i = 0; i < importedMesh->mNumVertices; i++)
	{
		normals.push_back(toGLM(importedMesh->mNormals[i]));

		if (i >= importedMesh->mNumVertices / 2)
		{
			log(YELLOW) << "\rNormals: 50%";
		}
	}
	log(GREEN) << "\rNormals: 100%" << std::endl;
}

void processIndices(std::vector<glm::uvec3>& faces, std::vector<GLuint>& indices, const aiMesh* importedMesh)
{
	log(YELLOW) << "Indices: 0%";
	faces.reserve(faces.capacity() + importedMesh->mNumFaces);
	indices.reserve(indices.capacity() + importedMesh->mNumFaces * 3);
	for (int i = 0; i < importedMesh->mNumFaces; i++)
	{
		// We assume all faces only have 3 vertices.
		glm::uvec3 face;
		face[0] = importedMesh->mFaces[i].mIndices[0];
		face[1] = importedMesh->mFaces[i].mIndices[1];
		face[2] = importedMesh->mFaces[i].mIndices[2];
		faces.push_back(face);

		indices.push_back(face[0]);
		indices.push_back(face[1]);
		indices.push_back(face[2]);

		if (i >= importedMesh->mNumVertices / 2)
		{
			log(YELLOW) << "\rIndices: 50%";
		}
	}
	log(GREEN) << "\rIndices: 100%" << std::endl;
}

struct Header
{
public:
	uint32_t magicNumber	= 149;
	uint32_t numVertices	= 0;
	uint32_t numFaces		= 0;
	uint32_t nameLength		= 0;

private:
};

static_assert(sizeof(Header) <= HEADER_RESERVED_BYTES, "Header has exceeded reserved bytes!");

void writeHeader(Header* header, FILE* file)
{
	fwrite(header, sizeof(Header), 1, file);
}

void testRead(const char* fileName);

bool convertFile(char* filePath)
{
	const aiScene* importedData = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	if (importedData == nullptr)
	{
		log(RED) << "FAILED TO PROCESS: " << getNameOfFile(filePath) << std::endl;
		return false;
	}

	log(GREEN) << "Processing " << importedData->mNumMeshes << " meshes" << std::endl;

	std::string name;
	unsigned int textureIndex = 0;

	std::vector<glm::vec3>		positions;
	std::vector<glm::vec3>		texCoords;
	std::vector<glm::vec3>		normals;
	std::vector<glm::uvec3>		faces;
	std::vector<GLuint>			indices;
	

	for (int meshNum = 0; meshNum < importedData->mNumMeshes; meshNum++)
	{
		const aiMesh* importedMesh = importedData->mMeshes[meshNum];

		name = std::string(importedMesh->mName.C_Str());
		textureIndex = importedMesh->mMaterialIndex;

		processPositions(positions, importedMesh);
		processTexCoords(texCoords, importedMesh);
		processNormals(normals, importedMesh);
		processIndices(faces, indices, importedMesh);
	}
	
	std::string outputFilePrefix = getPathExceptEnding(filePath);
	std::string mainOutput = outputFilePrefix + ".kmf";

	FILE* file = nullptr;
	fopen_s(&file, mainOutput.c_str(), "wb");

	if (file == nullptr)
	{
		log(RED) << "FAILED TO CREATE FILE!" << std::endl;
		return false;
	}

	Header header;
	header.numVertices	= (uint32_t)positions.size();
	header.numFaces		= (uint32_t)faces.size();
	header.nameLength	= (uint32_t)name.size();

	writeHeader(&header, file);

	fwrite(name.data(), sizeof(name[0]), name.size(), file);
	fwrite(positions.data(), sizeof(positions[0]), positions.size(), file);
	fwrite(texCoords.data(), sizeof(texCoords[0]), texCoords.size(), file);
	fwrite(normals.data(), sizeof(normals[0]), normals.size(), file);
	fwrite(faces.data(), sizeof(faces[0]), faces.size(), file);

	fclose(file);

	//testRead(mainOutput.c_str());

	aiReleaseImport(importedData);

	return true;
}

#define read_buffer(buffer) fread_s(&buffer[0], buffer.size() * sizeof(buffer[0]), sizeof(buffer[0]), buffer.size(), in)

void testRead(const char* fileName)
{
	std::string name;

	std::vector<glm::vec3>		positions;
	std::vector<glm::vec3>		texCoords;
	std::vector<glm::vec3>		normals;
	std::vector<glm::uvec3>		faces;
	std::vector<GLuint>			indices;

	Header test;

	FILE* in;
	fopen_s(&in, fileName, "rb");

	fread_s(&test, sizeof(Header), sizeof(Header), 1, in);
	
	name.resize(test.nameLength);
	positions.resize(test.numVertices);
	texCoords.resize(test.numVertices);
	normals.resize(test.numVertices);
	faces.resize(test.numFaces);
	indices.resize(test.numFaces * 3);

	read_buffer(name);

	read_buffer(positions);
	read_buffer(texCoords);
	read_buffer(normals);
	read_buffer(faces);

	//fread_s(positions.data(), positions.size() * sizeof(positions[0]), sizeof(positions[0]), test.numVertices, in);

}

int main(int argc, char* argv[])
{
	// Initialize log
	gConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(gConsoleHandle, &gPreviousState);
	
	if (argc > 1)
	{
		int successCount = 0;
		for (int i = 1; i < argc; i++)
		{
			log(GREEN) << "-------- Beginning work on: " << getNameOfFile(argv[i]) << " --------" << std::endl;
			
			if (convertFile(argv[i]))
			{
				successCount++;
			}
		}

		LogColor color = GREEN;
		if (successCount == 0)
		{
			color = RED;
		}
		else if (successCount != argc - 1)
		{
			color = YELLOW;
		}

		log(color) << "--------" << std::endl;
		log(color) << "Finished! " << successCount << " out of " << argc - 1 << " succeeded!" << std::endl;
	}
	else
	{
#if _DEBUG
		convertFile("test2.fbx");
#endif

		log(RED) << "No file was given as input" << std::endl;
	}

	std::cin.get();

	// Clean up log
	SetConsoleTextAttribute(gConsoleHandle, gPreviousState.wAttributes);
}