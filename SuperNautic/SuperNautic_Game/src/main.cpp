//SuperNautic

#include <iostream>

#include <GL\glew.h>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <crtdbg.h> // Leak-checking

#include "Log.hpp"
#include "Core\Game.hpp"





#include "Core\Track\SegmentHandler.hpp"
#include "Core\Track\Segment.hpp"


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;

	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK)
	{
		LOG_ERROR("Failed to initialize glew!");
	}
	else
	{
		SegmentHandler testSH{ "Segments/segmentinfos.txt" };
		const Segment* mySegment = testSH.loadSegment(0);

		game.bInitialize();
		game.run();
	}


	return 0;
}