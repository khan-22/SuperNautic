#include <iostream>

#include <Windows.h>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <vector>

#include "stringFuncs.h"
#include "mathConverterFuncs.h"
#include "fileOutputFuncs.h"

// Globals
HANDLE gConsoleHandle;
CONSOLE_SCREEN_BUFFER_INFO gPreviousState;
//bool gTransformCoordinates;
glm::mat4 gCoordinateTransform = glm::mat4(
	1.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, -1.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 1.f);

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





void processPositions(std::vector<glm::vec3>& positions, const aiMesh* importedMesh, const glm::mat4& transform)
{
	log(YELLOW) << "Positions: 0%";
	positions.reserve(positions.capacity() + importedMesh->mNumVertices);
	for (unsigned int i = 0; i < importedMesh->mNumVertices; i++)
	{
		glm::vec3 pos = toGLM(importedMesh->mVertices[i]);
		glm::vec3 transformedPos = glm::vec3(transform * glm::vec4(pos, 1.f));
		positions.push_back(transformedPos);

		//if (gTransformCoordinates)
		//{
		//	transformedPos = glm::vec3(gCoordinateTransform * glm::vec4(transformedPos, 1.f));
		//}

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
		for (unsigned int i = 0; i < importedMesh->mNumVertices; i++)
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
		for (unsigned int i = 0; i < importedMesh->mNumVertices; i++)
		{
			texCoords.push_back({ 0.f, 0.f, 0.f });
		}
	}
	log(GREEN) << "\rTexCoord: 100%" << std::endl;
}

void processNormals(std::vector<glm::vec3>& normals, const aiMesh* importedMesh, const glm::mat4& transform)
{
	log(YELLOW) << "Normals: 0%";
	normals.reserve(normals.capacity() + importedMesh->mNumVertices);
	for (unsigned int i = 0; i < importedMesh->mNumVertices; i++)
	{
		glm::vec3 normal = toGLM(importedMesh->mNormals[i]);
		glm::vec3 transformedNormal = glm::normalize(glm::vec3(transform * glm::vec4(normal, 0.f)));

		//if (gTransformCoordinates)
		//{
		//	transformedNormal = glm::vec3(gCoordinateTransform * glm::vec4(transformedNormal, 1.f));
		//}

		normals.push_back(transformedNormal);

		if (i >= importedMesh->mNumVertices / 2)
		{
			log(YELLOW) << "\rNormals: 50%";
		}
	}
	log(GREEN) << "\rNormals: 100%" << std::endl;
}

void processIndices(std::vector<glm::uvec3>& faces, std::vector<GLuint>& indices, const aiMesh* importedMesh, unsigned int& indexOffset)
{
	log(YELLOW) << "Indices: 0%";
	faces.reserve(faces.capacity() + importedMesh->mNumFaces);
	indices.reserve(indices.capacity() + importedMesh->mNumFaces * 3);

	unsigned int largestIndex = 0;
	for (unsigned int i = 0; i < importedMesh->mNumFaces; i++)
	{
		// We assume all faces only have 3 vertices.
		glm::uvec3 face;
		face[0] = importedMesh->mFaces[i].mIndices[0] + indexOffset;
		face[1] = importedMesh->mFaces[i].mIndices[1] + indexOffset;
		face[2] = importedMesh->mFaces[i].mIndices[2] + indexOffset;
		faces.push_back(face);

		indices.push_back(face[0]);
		indices.push_back(face[1]);
		indices.push_back(face[2]);

		largestIndex = std::max(std::max(std::max(face[0], face[1]), face[2]), largestIndex);

		if (i >= importedMesh->mNumVertices / 2)
		{
			log(YELLOW) << "\rIndices: 50%";
		}
	}
	log(GREEN) << "\rIndices: 100%" << std::endl;

	indexOffset = largestIndex + 1;
}


void testRead(const char* fileName);

Mesh* accumulatedMesh = nullptr;

void processNodeMeshes(const aiScene* scene, const aiNode* currentNode, std::vector<Mesh>& meshes, glm::mat4 accumulatedTransform)
{
	int numMeshes = currentNode->mNumMeshes;

	accumulatedTransform = accumulatedTransform * toGLM(currentNode->mTransformation);

	const char* previousName = "";

	unsigned int indexOffset = 0;

	for (int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		const aiMesh* importedMesh = scene->mMeshes[currentNode->mMeshes[meshIndex]];

		log(GREEN) << "-- Processing Mesh: " << currentNode->mName.C_Str() << std::endl;

		Mesh* current;
		if (strcmp(previousName, currentNode->mName.C_Str()) == 0)
		{
			log(GREEN) << "Name is the same as previous, adding to it" << std::endl;

			current = &meshes.back();
		}
		else if (containsSpecialName(currentNode->mName.C_Str()))
		{
			meshes.emplace_back();
			current = &meshes.back();
			indexOffset = 0;
		}
		else
		{
			log(GREEN) << "Name not recognized, adding to accumulated mesh" << std::endl;
			
			if (meshes.size() == 0)
			{
				meshes.emplace_back();
				accumulatedMesh = &meshes.back();
				indexOffset = 0;
			}
			else
			{
				if (accumulatedMesh == nullptr)
				{
					meshes.emplace_back();
					accumulatedMesh = &meshes.back();
					indexOffset = 0;
				}

			}
			
			current = accumulatedMesh;
		}

		current->name = std::string(currentNode->mName.C_Str());
		current->textureIndex = importedMesh->mMaterialIndex;

		glm::mat4 normalTransformation = glm::transpose(glm::inverse(accumulatedTransform));


		processPositions(current->positions, importedMesh, accumulatedTransform);
		processTexCoords(current->texCoords, importedMesh);
		processNormals(current->normals, importedMesh, normalTransformation);
		processIndices(current->faces, current->indices, importedMesh, indexOffset);

		previousName = currentNode->mName.C_Str();
	}
	
	int numChildren = currentNode->mNumChildren;
	for (int nodeIndex = 0; nodeIndex < numChildren; nodeIndex++)
	{
		processNodeMeshes(scene, currentNode->mChildren[nodeIndex], meshes, accumulatedTransform);
	}
	
}

void processNodeCameras(const aiScene* scene, std::vector<Camera>& cameras)
{
	int numCameras = scene->mNumCameras;
	if (numCameras > 1)
	{
		log(YELLOW) << "The converter can currently not handle more than one camera.\nShould there really be more than one?" << std::endl;
	}

	const aiCamera* camera	 = scene->mCameras[0];
	const aiNode* cameraNode = scene->mRootNode->FindNode(camera->mName);


	log(GREEN) << "-- Processing Camera: " << cameraNode->mName.C_Str() << std::endl;
	
	glm::mat4 cameraTransform = toGLM(cameraNode->mTransformation);
	/*
	glm::vec3 pos	 = glm::vec3( cameraTransform * glm::vec4(toGLM(camera->mPosition), 1.f));
	glm::vec3 lookAt = glm::vec3( cameraTransform * glm::vec4(toGLM(camera->mLookAt), 0.f));
	glm::vec3 up	 = glm::vec3( cameraTransform * glm::vec4(toGLM(camera->mUp), 0.f));
	*/
	
	glm::vec3 pos	 = toGLM(camera->mPosition) / 100.f;
	glm::vec3 lookAt = toGLM(camera->mLookAt);
	glm::vec3 up	 = toGLM(camera->mUp);

	glm::mat4 endMatrix = glm::inverse(glm::lookAt(pos, pos + glm::normalize(lookAt), up));

	/*if (gTransformCoordinates)
	{
		cameraTransform = gCoordinateTransform * cameraTransform;
	}*/
	glm::vec3 temp = glm::normalize(lookAt);
	log(GREEN) << "Camera lookAt: " << temp.x << ", " << temp.y << ", " << temp.z << ", " << " " << std::endl;

	Camera finalCamera;
	finalCamera.transform = endMatrix;

	glm::vec4 camPos = finalCamera.transform * glm::vec4(0.f, 0.f, 0.f, 1.f);
	log(GREEN) << "Camera location: " << camPos.x << ", " << camPos.y << ", " << camPos.z << ", " << camPos.w << " " << std::endl;
	
	cameras.push_back(finalCamera);
}

bool convertFile(char* filePath)
{
	const aiScene* importedData = aiImportFile(filePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);

	if (importedData == nullptr)
	{
		log(RED) << "FAILED TO PROCESS: " << getNameOfFile(filePath) << std::endl;
		return false;
	}

	const aiNode*  rootNode     = importedData->mRootNode;

	log(GREEN) << "Processing " << importedData->mNumMeshes << " meshes" << std::endl;
	log(YELLOW) << "Processing " << importedData->mNumLights << " lights (NOT IMPLEMENTED!)" << std::endl;
	log(YELLOW) << "Processing " << importedData->mNumCameras << " cameras (only one is handled)" << std::endl;

	// Open the output file...
	std::string outputFilePrefix = getPathExceptEnding(filePath);
	std::string mainOutput = outputFilePrefix + ".kmf";

	FILE* file = nullptr;
	fopen_s(&file, mainOutput.c_str(), "wb");

	if (file == nullptr)
	{
		log(RED) << "FAILED TO CREATE FILE!" << std::endl;
		return false;
	}

	// Process each mesh

	std::vector<Mesh>	meshes;
	std::vector<Light>	lights;
	std::vector<Camera>	cameras;

	// Recursive processing of scene node tree structure.
	if (importedData->HasMeshes())
	{
		processNodeMeshes(importedData, importedData->mRootNode, meshes, glm::mat4(1.f));
	}
	if (importedData->HasLights())
	{

	}
	if (importedData->HasCameras())
	{
		processNodeCameras(importedData, cameras);

	}
	
	Header header;
	header.numMeshes  = static_cast<uint32_t>(meshes.size());
	header.numLights  = static_cast<uint32_t>(lights.size());
	header.numCameras = static_cast<uint32_t>(cameras.size());

	writeHeader(&header, file);

	// Write Meshes
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh& current = meshes[i];

		MeshHeader meshHeader;
		meshHeader.numVertices	= static_cast<uint32_t>(current.positions.size());
		meshHeader.numFaces		= static_cast<uint32_t>(current.faces.size());
		meshHeader.nameLength	= static_cast<uint32_t>(current.name.size());

		writeHeader(&meshHeader, file);
		writeData(&current, file);
	}

	// Write Lights
	for (int i = 0; i < lights.size(); i++)
	{
		// There is no lights header

		writeData(&lights[i], file);
	}

	// Write Cameras
	for (int i = 0; i < cameras.size(); i++)
	{
		// There is no camera header

		writeData(&cameras[i], file);
	}

	//fwrite(name.data(), sizeof(name[0]), name.size(), file);
	//fwrite(positions.data(), sizeof(positions[0]), positions.size(), file);
	//fwrite(texCoords.data(), sizeof(texCoords[0]), texCoords.size(), file);
	//fwrite(normals.data(), sizeof(normals[0]), normals.size(), file);
	//fwrite(faces.data(), sizeof(faces[0]), faces.size(), file);

	fclose(file);

	//testRead(mainOutput.c_str());

	aiReleaseImport(importedData);

	return true;
}

#define read_buffer(buffer) fread_s(&buffer[0], buffer.size() * sizeof(buffer[0]), sizeof(buffer[0]), buffer.size(), in)

void testRead(const char* fileName)
{
	/*std::string name;

	std::vector<glm::vec3>		positions;
	std::vector<glm::vec3>		texCoords;
	std::vector<glm::vec3>		normals;
	std::vector<glm::uvec3>		faces;
	std::vector<GLuint>			indices;
*/

	std::vector<Mesh> meshes;
	std::vector<Light> lights;
	std::vector<Camera> cameras;
	Header header;

	FILE* in;
	fopen_s(&in, fileName, "rb");

	fread_s(&header, sizeof(Header), sizeof(Header), 1, in);

	for (unsigned int i = 0; i < header.numMeshes; i++)
	{
		MeshHeader meshHeader;
		
		fread_s(&meshHeader, sizeof(MeshHeader), sizeof(MeshHeader), 1, in);

		meshes.emplace_back();
		Mesh& current = meshes.back();

		current.name.resize(meshHeader.nameLength);
		current.positions.resize(meshHeader.numVertices);
		current.texCoords.resize(meshHeader.numVertices);
		current.normals.resize(meshHeader.numVertices);
		current.faces.resize(meshHeader.numFaces);
		current.indices.resize(meshHeader.numFaces * 3);

		read_buffer(current.name);
		read_buffer(current.positions);
		read_buffer(current.texCoords);
		read_buffer(current.normals);
		read_buffer(current.faces);

		// Go back to read index data again
		fseek(in, -(int)(current.faces.size() * sizeof(current.faces[0])), SEEK_CUR);

		read_buffer(current.indices);
	}

	for (unsigned int i = 0; i < header.numLights; i++)
	{
		lights.emplace_back();
		Light& current = lights.back();
		
		// Does nothing yet...
	}

	for (unsigned int i = 0; i < header.numCameras; i++)
	{
		cameras.emplace_back();
		Camera& current = cameras.back();

		fread_s(&current.transform, sizeof(current.transform), sizeof(current.transform), 1, in);
	}
	
	/*name.resize(test.nameLength);
	positions.resize(test.numVertices);
	texCoords.resize(test.numVertices);
	normals.resize(test.numVertices);
	faces.resize(test.numFaces);
	indices.resize(test.numFaces * 3);*/

	/*read_buffer(name);

	read_buffer(positions);
	read_buffer(texCoords);
	read_buffer(normals);
	read_buffer(faces);*/

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

			if (strstr(argv[i], ".blend") != nullptr)
			{
				log(RED) << "Warning! .blend files may not be correctly converted!" << std::endl;
			}

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
		convertFile("s03_10degbend_helper_aa.fbx");
		//gTransformCoordinates = true;
#endif

		log(RED) << "No file was given as input" << std::endl;
	}

	std::cin.get();

	// Clean up log
	SetConsoleTextAttribute(gConsoleHandle, gPreviousState.wAttributes);
}