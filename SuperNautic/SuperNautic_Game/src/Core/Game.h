#pragma once

#ifndef GAME_H
#define GAME_H

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"

class Game
{
public:
	Game();
	~Game();

	void run();

	void handleEvents();
	void update(float dt);
	void render();

protected:


private:
	//sf::Window	_window;
	sf::RenderWindow _window;
	ApplicationStateStack _stateStack;
	ApplicationContext _context;


};

#endif //GAME_H
