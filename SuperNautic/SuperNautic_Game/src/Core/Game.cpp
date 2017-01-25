#include "Game.h"

#include <SFML/OpenGL.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


#include "../Log.h"
#include "../GFX/VertexDataImporter.h"

Game::Game()
	: _window(sf::VideoMode(800, 600), "Test window", sf::Style::Default, sf::ContextSettings(0U, 0U, 0U, 4U, 0U))
{
	LOG("Game is being constructed...");

	glEnable(GL_DEPTH_TEST);

	GFX::VertexDataImporter importer("./res/models/");
	auto test = importer.importVertexData("test.fbx");

	// Test importing a model
	//const aiScene* testModel = nullptr;
	//if ((testModel = aiImportFile("./res/models/test.fbx", aiProcessPreset_TargetRealtime_MaxQuality)) == nullptr)
	//{
	//	LOG_ERROR("FAILED TO LOAD TEST.FBX");
	//}
	//else
	//{
	//	LOG("Num meshes in test file: ", testModel->mNumMeshes);
	//	LOG("Num vertices in mesh 1: ", testModel->mMeshes[0]->mNumVertices);
	//	LOG("Size of aiVector3D: ", sizeof(testModel->mMeshes[0]->mVertices[0]));
	//	LOG("Size of glm::vec3: ", sizeof(glm::vec3));
	//	LOG("Size of aiVector3D: ", sizeof(testModel->mMeshes[0]->mTextureCoords[0]));
	//	LOG("Size of glm::vec2: ", sizeof(glm::vec2));
	//}
	
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
