#pragma once

#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>

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
	sf::Window	_window;
	


};

#endif //GAME_H
