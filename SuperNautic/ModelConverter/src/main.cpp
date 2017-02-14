#include <iostream>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <Windows.h>

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

int main()
{
	// Initialize log
	gConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleScreenBufferInfo(gConsoleHandle, &gPreviousState);
	
	// ---

	log(WHITE) << "Hello World! TEST" << std::endl;

	const aiScene* importedData = aiImportFile("./test.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	
	log(GREEN) << "Loaded: " << importedData->mNumMeshes << " meshes" << std::endl;

	glm::vec3 test(3.f, 5.f, 5.f);
	glm::vec3 test2(5.f, 5.f, 5.f);

	std::cout << test2.y - test.x << std::endl;

	aiReleaseImport(importedData);

	GLfloat t = 0.f;

	std::cout << t << std::endl;
	
	std::cin.get();

	// Clean up log
	SetConsoleTextAttribute(gConsoleHandle, gPreviousState.wAttributes);
}