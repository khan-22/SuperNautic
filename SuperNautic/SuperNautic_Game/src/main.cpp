//SuperNautic

#include <iostream>

#include <GL\glew.h>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <crtdbg.h> // Leak-checking

#include "Log.hpp"
#include "Core\Game.hpp"

// For ray intersection testing
#include "Core\Geometric Primitives\RayIntersection.hpp"
#include "Core\Geometric Primitives\Ray.hpp"
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
		// Test ray intersection
		SegmentHandler sh{ "Segments/segmentinfos.txt" };
		const Segment* s{ sh.loadSegment(0) };
		Ray r{ glm::vec3{0,0,-2}, glm::vec3{-1,0,-0.5}, 10000.0f };
		RayIntersection i = s->rayIntersectionTest(r);

		game.bInitialize();
		game.run();
	}


	return 0;
}