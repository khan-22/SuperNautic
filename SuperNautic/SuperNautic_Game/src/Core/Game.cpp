#include "Game.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

#include "../Log.hpp"
#include "../GFX/VertexDataImporter.hpp"
#include "MainMenuApplicationState.hpp"
#include "../GFX/TexturedModel.hpp"

#include "LoadAssetFunctions.hpp"

#include "../GFX/ShaderLoader.hpp"

#include "Geometric Primitives\RayIntersection.hpp"
#include "Geometric Primitives\Ray.hpp"
#include "Track\SegmentHandler.hpp"
#include "Track\Segment.hpp"

#include "..\GFX\Transformable3D.hpp"



Game::Game()
	: _window(sf::VideoMode(1280, 720), "Test window", sf::Style::Default, sf::ContextSettings(24U, 0U, 0U, 4U, 0U))
	, _context(_window)
	, _quitTimer(0.f)
	, _fps(60.f)
	, _camera(90.f, 1280, 720, glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
	, _debugCamera(90.f, 1280, 720, glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 0.f, 1.f))
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

	GLint vertexUniCount = 0;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &vertexUniCount);
	GLint geoUniCount = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &geoUniCount);
	GLint fragUniCount = 0;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &fragUniCount);
	LOG("[VERTEX] There are ", vertexUniCount, " available uniform locations");
	LOG("[GEOMETRY] There are ", geoUniCount, " available uniform locations");
	LOG("[FRAGMENT] There are ", fragUniCount, " available uniform locations");
//
//	// Cached asset loading **DEMO**
	/*RawMeshAsset testRawMesh = RawMeshCache::get("Segments/s01_straight_aa.fbx");

	if (testRawMesh.get() == nullptr)
	{
		LOG("Failed to load model... Oopsie poopsie!");
	}
	else
	{
		LOG("The loaded mesh has: ", testRawMesh.get()->meshes[0].vertices.size(), " vertices");
	}*/

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

    TextureAsset textureTest = TextureCache::get("heatchart.png");
    if(textureTest.get() == nullptr)
    {
        LOG("Failed to load texture.");
    }

    MaterialAsset materialTest = MaterialCache::get("test.mat");
    if(materialTest.get() == nullptr)
    {
        LOG("Failed to load material.");
    }

	//_track.setLength(10000);
	//_track.setSeed(1);
	//_track.generate();



	_model = ModelCache::get("ship.fbx");
	/*We can create a loop here (or where relevant) that loops through a list
	of all the things we want to render and add them to the model array, such as
	segments, ships etc.*/
	ModelArray.push_back(ModelCache::get("ship.fbx"));
	////ModelArray.push_back(ModelCache::get("segments/s01_straight_aa.fbx"));
	//for (unsigned int i = 0; i < _track.getNrOfSegments(); i++)
	//{
	//	ModelArray.push_back(_track.getMeshBySegmentIndex(i));
	//}


	_shader = ShaderCache::get("forward");
	_texture = TextureCache::get("heatchart.png");

	_texturedModel.setModelAndMaterial(ModelArray[0], materialTest);

	_forwardRenderer.initialize(&_window, 0.0f, 0.0f, 1.0f, 1.0f);
	_deferredRenderer1.initialize(&_window, 0.0f, 0.0f, 1.0f, 1.0f);
	//_deferredRenderer2.initialize(&_window, 0.5f, 0.0f, 0.5f, 0.5f);
	//_deferredRenderer3.initialize(&_window, 0.0f, 0.5f, 0.5f, 0.5f);
	//_deferredRenderer4.initialize(&_window, 0.5f, 0.5f, 0.5f, 0.5f);

	std::unique_ptr<ApplicationState> mainMenu(new MainMenuApplicationState(_stateStack, _context));
	_stateStack.push(mainMenu);

	
	_pointLights.push_back(PointLight({ 0.f,2.f,0.f }, { 1.f, 0.f, 0.f }, 1.0f));
	_pointLights.push_back(PointLight({ 6.f,0.f,0.f }, { 0.f, 1.f, 0.f }, 1.f));
	_pointLights.push_back(PointLight({ 0.f,2.f,6.f }, { 0.f, 0.f, 1.f }, 1.f));

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

        default:
            _stateStack.handleEvent(event);
            break;
		}
	}

}

void Game::update(float dt)
{
    _fps = _fps * 0.9f + 0.1f / dt;

	//_debugCamera.update(dt, _window);

	static float t = 0.0;
	t += dt;
	_pointLights[0].setPosition(glm::vec3(2.f * sinf(t*4.f), 2.f, 2.f * cosf(t*4.f)));

    _stateStack.update(dt);
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	static float time = 0.f;
	time += 0.009f;

	_camera.setPos(glm::vec3(0.f, 0.f, 5.f));
	_camera.setViewDir(glm::vec3(0.f, 0.f, -1.f));

	for (auto& pointLight : _pointLights)
	{
		_deferredRenderer1.pushPointLight(pointLight);
	}

	_deferredRenderer1.render(_texturedModel);
	_deferredRenderer1.display(_debugCamera);

	//_deferredRenderer2.render(*_model.get());
	//_deferredRenderer3.render(*_model.get());
	//_deferredRenderer4.render(*_model.get());



	/*glm::mat4 model(1.f);
	glm::mat4 view = glm::lookAt(glm::vec3{ 20.f * sinf(time), 0.f, 20.f * cosf(time) }, glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ 0.f, 1.f, 0.f });
	glm::mat4 projection = glm::perspective(70.f, (float)_window.getSize().x / (float)_window.getSize().y, 0.3f, 100.f);

	glm::vec4 color(1.f, 0.f, 0.f, 1.f);

	_shader.get()->setUniform("uModel", model);
	_shader.get()->setUniform("uView", view);
	_shader.get()->setUniform("uProjection", projection);

	_shader.get()->setUniform("uColor", color);

	_model.get()->render();*/


	_camera.setPos(glm::vec3(0.f, 0.f, -5.f));//glm::vec3(20.f * sinf(time), 0.f, 20.f * cosf(time)));
    //_forwardRenderer.render(*_model.get());
	//_forwardRenderer.render(_texturedModel);
	//_forwardRenderer.display(_debugCamera);
	//LOG_GL_ERRORS();

    static Asset<sf::Font> font = AssetCache<sf::Font, std::string>::get("res/arial.ttf");
    sf::Text fps;
    fps.setFont(*font.get());
    fps.setString("FPS: " + std::to_string(_fps));

    _stateStack.render();

    _window.pushGLStates();
    _window.draw(fps);
    _window.popGLStates();

	_window.display();
}
