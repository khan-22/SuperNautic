//SuperNautic

#include <iostream>

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <crtdbg.h> // Leak-checking

#include "Core/Io/Log.hpp"
#include "Core/Game.hpp"

// For ray intersection testing
#include "Core/Geometry/RayIntersection.hpp"
#include "Core/Geometry/Ray.hpp"
#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/Segment.hpp"

#include "GFX/Rendering/Transformable3D.hpp"


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
		game.bInitialize();
		game.run();
	}


	return 0;
}