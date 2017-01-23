#include "Game.h"

#include "../Log.h"

Game::Game()
	: _window(sf::VideoMode(800, 600), "Test window")
{
	LOG("Game is being constructed...");


}

Game::~Game()
{

}

void Game::run()
{


}

void Game::handleEvents()
{
}

void Game::update(float dt)
{
}

void Game::render()
{
}
