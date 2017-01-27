#include "Game.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Log.h"
#include "../GFX/VertexDataImporter.h"
#include "PlayApplicationState.hpp"

#include "AssetCache.hpp"

#include "../GFX/ShaderLoader.h"

Game::Game()
	: _window(sf::VideoMode(800, 600), "Test window", sf::Style::Default, sf::ContextSettings(0U, 0U, 0U, 4U, 0U))
	, _context(_window)
{
	LOG("Game is being constructed...");
	

}

Game::~Game()
{
	LOG("Game is being destructed...");

	CLOSE_LOG();
}

bool Game::bInitialize()
{
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

	// Shader loading **DEMO**
	//GFX::ShaderLoader shaderLoader("./src/GFX/Shaders/");
	//GFX::Shader* testShader = shaderLoader.loadShader("forward");
	AssetCache<GFX::Shader, std::string> shaderCache;
	Asset<GFX::Shader> testShader = shaderCache.get("forward");
	
	if (testShader.get() == 0)
	{
		LOG("Failed to load shader... Oopsie poopsie!");
	}
	else
	{
		LOG("The loaded shader has ID: ", testShader.get()->shaderProgram);
	}

	std::unique_ptr<ApplicationState> playState(new PlayApplicationState(_stateStack, _context));
	_stateStack.push(playState);

	return true;
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
			else
            {
                _stateStack.handleEvent(event);
            }
			break;

        default:
            _stateStack.handleEvent(event);
            break;
		}
	}

}

void Game::update(float dt)
{
    _stateStack.update(dt);
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _stateStack.render();
	_window.display();
}
