//SuperNautic

#include <iostream>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <crtdbg.h> // Leak-checking

#include "Log.h"
#include "Core\Game.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;
	game.run();
	
	return 0;
}