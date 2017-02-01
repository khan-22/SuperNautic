#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"

#include "Player.hpp"

#include "LoadAssetFunctions.hpp"

#include <windows.h>

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

	std::list<Player>		_players;

	float					_quitTimer;
	float _fps;

	ShaderAsset	_shader;
	ModelAsset	_model;
	HDC _deviceContext3d;
	HGLRC _glContext3d;

	void activateGlContext3d(bool bDoActivate);

};

#endif //GAME_HPP
