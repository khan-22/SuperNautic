#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"

#include "Player.hpp"

class Game
{
public:
	Game();
	~Game();

	bool bInitialize();

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

	std::list<Player> _players;
};

#endif //GAME_HPP
