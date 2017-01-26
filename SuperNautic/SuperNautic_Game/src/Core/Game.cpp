#include "Game.h"

#include <SFML/OpenGL.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


#include "../Log.h"
#include "../GFX/VertexDataImporter.h"

#include "AssetCache.hpp"

Game::Game()
	: _window(sf::VideoMode(800, 600), "Test window", sf::Style::Default, sf::ContextSettings(0U, 0U, 0U, 4U, 0U))
{
	LOG("Game is being constructed...");
	
	glEnable(GL_DEPTH_TEST);


	// Cached asset loading **DEMO**
	AssetCache<GFX::RawMeshCollection, std::string> meshCache;

	Asset<GFX::RawMeshCollection> testModel = meshCache.get("test.fbx");

	if (testModel.get() == nullptr)
	{
		LOG("Failed to load model... Oopsie poopsie!");
	}
	else
	{
		LOG("The loaded mesh has: ", testModel.get()->meshes[0].vertices.size(), " vertices");
	}

	
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
