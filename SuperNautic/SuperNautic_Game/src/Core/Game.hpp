#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/World/Player.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"
#include "Core/Utility/DebugCamera.hpp"
#include "Core/Audio/WorldAudio.hpp"
#include "Core/Track/Track.hpp"
#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/Segment.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "GFX/Rendering/DeferredRenderer.hpp"
#include "GFX/Resources/TexturedModel.hpp"

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
	float					_fps;

	ShaderAsset				_shader;

	//TEST MODELS are put in this array for rendering
	//MAKE SURE NOT TO PUSH THIS TO MASTER, at least not without discussing
	std::vector<GFX::TexturedModel> ModelArray;
	//////////////////////

	TextureAsset			_texture;
	GFX::TexturedModel		_texturedModel;

	GFX::ForwardRenderer	_forwardRenderer;
	GFX::DeferredRenderer	_deferredRenderer;

	Camera					_camera;
	DebugCamera				_debugCamera;

	//SegmentHandler			_segmentHandler;
	//Track					_track;
	//Camera		_camera;
	//DebugCamera _debugCamera;

	std::vector<PointLight>	_pointLights;

	WorldAudio				_audio;
};

#endif //GAME_HPP
