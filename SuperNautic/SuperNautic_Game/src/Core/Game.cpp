#include "Game.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

#include "../Log.hpp"
#include "../GFX/VertexDataImporter.hpp"
#include "MainMenuApplicationState.hpp"

#include "LoadAssetFunctions.hpp"

#include "../GFX/ShaderLoader.hpp"



Game::Game()
	: _window(sf::VideoMode(1280, 720), "Test window", sf::Style::Default, sf::ContextSettings(24U, 0U, 0U, 4U, 0U))
	, _context(_window)
	, _quitTimer(0.f)
	, _fps(60.f)
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
	glCullFace(GL_BACK);
//
//	// Cached asset loading **DEMO**
	RawMeshAsset testRawMesh = RawMeshCache::get("Segments/s01_straight_aa.fbx");

	if (testRawMesh.get() == nullptr)
	{
		LOG("Failed to load model... Oopsie poopsie!");
	}
	else
	{
		LOG("The loaded mesh has: ", testRawMesh.get()->meshes[0].vertices.size(), " vertices");
	}

	// Shader loading **DEMO**
	/*GFX::ShaderLoader shaderLoader("./src/GFX/Shaders/");
	GFX::Shader* testShader = shaderLoader.loadShader("forward");*/

	Asset<GFX::Shader> testShader = ShaderCache::get("forward");

	if (testShader.get() == 0)
	{
		LOG("Failed to load shader... Oopsie poopsie!");
	}
	else
	{
		LOG("The test shader has been loaded!");
	}

	// Model loading **DEMO**
	ModelAsset testModel = ModelCache::get("test2.fbx");

	if (testModel.get() == nullptr)
	{
		LOG("Failed to load model to GPU... :,(((");
	}
	else
	{
		LOG("WOOOOOW!!");
	}

	_model = ModelCache::get("test2.fbx");
	_shader = ShaderCache::get("forward");



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

//		if(_stateStack.bIsEmpty())
//        {
//            _window.close();
//        }

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

        default:
            _stateStack.handleEvent(event);
            break;
		}
	}

}

void Game::update(float dt)
{
    _fps = _fps * 0.9f + 0.1f / dt;

    _stateStack.update(dt);
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GFX::Shader* shader = _shader.get();
	_shader.get()->bind();

	static float time = 0.f;
	time += 0.004f;

	glm::mat4 model(1.f);
	glm::mat4 view = glm::lookAt(glm::vec3{ 3.f * sinf(time), 0.f, 3.f * cosf(time) }, glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ 0.f, 1.f, 0.f });
	glm::mat4 projection = glm::perspective(90.f, (float)_window.getSize().x / (float)_window.getSize().y, 0.3f, 100.f);

	glm::vec4 color(1.f, 0.f, 0.f, 1.f);

	_shader.get()->setUniform("uModel", model);
	_shader.get()->setUniform("uView", view);
	_shader.get()->setUniform("uProjection", projection);

	_shader.get()->setUniform("uColor", color);

	_model.get()->render();


    static Asset<sf::Font> font = AssetCache<sf::Font, std::string>::get("res/arial.ttf");
    sf::Text fps;
    fps.setFont(*font.get());
    fps.setString("FPS: " + std::to_string(_fps));

    _window.pushGLStates();
    _stateStack.render();
    _window.draw(fps);
    _window.popGLStates();

	_window.display();
}
