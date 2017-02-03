#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"

#include "Player.hpp"

#include "LoadAssetFunctions.hpp"
#include "../GFX/ForwardRenderer.hpp"
#include "../GFX/TexturedModel.hpp"

#include "DebugCamera.hpp"

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
	sf::RenderWindow		_window;
	ApplicationStateStack	_stateStack;
	ApplicationContext		_context;

	float					_quitTimer;
	float _fps;

	ShaderAsset	_shader;
	//TEST MODELS
	ModelAsset	_model;
	ModelAsset	_segmentModel;
	//////////////////////

	TextureAsset _texture;
	GFX::TexturedModel _texturedModel;

	GFX::ForwardRenderer	_forwardRenderer;
	Camera		_camera;
	DebugCamera _debugCamera;
};

#endif //GAME_HPP
