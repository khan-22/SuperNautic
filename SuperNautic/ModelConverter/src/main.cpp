#include <iostream>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <GL/glew.h>

int main()
{
	std::cout << "Hello World!" << std::endl;

	const aiScene* importedData = aiImportFile("./test.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	
	std::cout << "Loaded: " << importedData->mNumMeshes << " meshes" << std::endl;

	glm::vec3 test(3.f, 5.f, 5.f);
	glm::vec3 test2(5.f, 5.f, 5.f);

	std::cout << test2.y - test.x << std::endl;

	aiReleaseImport(importedData);

	GLfloat t = 0.f;

	std::cout << t << std::endl;
	
	std::cin.get();
}