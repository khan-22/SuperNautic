#include "Game.h"

#include <SFML/OpenGL.hpp>

#include "../Log.h"

Game::Game()
	: _window(sf::VideoMode(800, 600), "Test window", sf::Style::Default, sf::ContextSettings(0U, 0U, 0U, 4U, 0U))
{
	LOG("Game is being constructed...");

	glEnable(GL_DEPTH_TEST);
}

Game::~Game()
{
	LOG("Game is being destructed...");

	CLOSE_LOG();
}

void Game::run()
{
	sf::Clock clock;
	sf::Time deltaTime = clock.restart();


	while (_window.isOpen())
	{
		handleEvents();
		update(deltaTime.asSeconds());
		render();

		deltaTime = clock.restart();
	}

}

void Game::handleEvents()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				_window.close();
			}
		}
	}

}

void Game::update(float dt)
{

}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_window.display();
}
