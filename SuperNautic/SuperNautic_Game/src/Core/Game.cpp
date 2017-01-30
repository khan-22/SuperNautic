#include "Game.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Log.hpp"
#include "../GFX/VertexDataImporter.hpp"
#include "MainMenuApplicationState.hpp"

#include "LoadAssetFunctions.hpp"

#include "../GFX/ShaderLoader.hpp"

Game::Game()
	: _window(sf::VideoMode(800, 600), "Test window", sf::Style::Default, sf::ContextSettings(0U, 0U, 0U, 4U, 0U))
	, _context(_window)
	, _quitTimer(0.f)
{
	LOG("Game is being constructed...");

	_players.emplace_back();
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
	Asset<GFX::RawMeshCollection> testRawMesh = AssetCache<GFX::RawMeshCollection, std::string>::get("test.fbx");

	if (testRawMesh.get() == nullptr)
	{
		LOG("Failed to load model... Oopsie poopsie!");
	}
	else
	{
		LOG("The loaded mesh has: ", testRawMesh.get()->meshes[0].vertices.size(), " vertices");
	}

	// Shader loading **DEMO**
	//GFX::ShaderLoader shaderLoader("./src/GFX/Shaders/");
	//GFX::Shader* testShader = shaderLoader.loadShader("forward");

	Asset<GFX::Shader> testShader = AssetCache<GFX::Shader, std::string>::get("forward");

	if (testShader.get() == 0)
	{
		LOG("Failed to load shader... Oopsie poopsie!");
	}
	else
	{
		LOG("The test shader has been loaded!");
	}

	// Model loading **DEMO**
	ModelAsset testModel = ModelCache::get("test.fbx");

	if (testModel.get() == nullptr)
	{
		LOG("Failed to load model to GPU... :,(((");
	}
	else
	{
		LOG("WOOOOOW!!");
	}

	std::unique_ptr<ApplicationState> mainMenu(new MainMenuApplicationState(_stateStack, _context));
	_stateStack.push(mainMenu);


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

		if(_stateStack.bIsEmpty())
        {
            _window.close();
        }

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
            LOG("Keep holding to shutdown...");
            _stateStack.handleEvent(event);
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Escape)
			{
				_quitTimer = 0.f;
			}

        default:
            _stateStack.handleEvent(event);
            break;
		}
	}

}

void Game::update(float dt)
{
    _stateStack.update(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		_quitTimer += dt;

		if (_quitTimer > 0.7f)
		{
			_window.close();
		}
	}
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _stateStack.render();
	_window.display();
}
