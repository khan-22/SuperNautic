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
#include "../GFX/DeferredRenderer.hpp"
#include "../GFX/TexturedModel.hpp"

#include "DebugCamera.hpp"

#include <vector>

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

	//TEST MODELS are put in this array for rendering
	//MAKE SURE NOT TO PUSH THIS TO MASTER, at least not without discussing
	std::vector<ModelAsset> ModelArray;
	//////////////////////

	// SHIP TESTING
	SegmentHandler sh;
	const Segment* s;
	Ship ship;
	///////////////

	TextureAsset _texture;
	GFX::TexturedModel _texturedModel;

	GFX::ForwardRenderer	_forwardRenderer;
	GFX::DeferredRenderer	_deferredRenderer1;
	//GFX::DeferredRenderer	_deferredRenderer2;
	//GFX::DeferredRenderer	_deferredRenderer3;
	//GFX::DeferredRenderer	_deferredRenderer4;
	Camera		_camera;
	DebugCamera _debugCamera;
};

#endif //GAME_HPP
