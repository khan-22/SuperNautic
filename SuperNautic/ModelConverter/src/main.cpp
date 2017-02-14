#include <iostream>

#include <Windows.h>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <GL/glew.h>


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
	gConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(gConsoleHandle, &gPreviousState);
	
	// ---

	log(WHITE) << "Hello World! TEST" << std::endl;

	const aiScene* importedData = aiImportFile("./test.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	
	log(GREEN) << "Loaded: " << importedData->mNumMeshes << " meshes" << std::endl;

	aiReleaseImport(importedData);

	std::cin.get();

	// Clean up log
	SetConsoleTextAttribute(gConsoleHandle, gPreviousState.wAttributes);
}