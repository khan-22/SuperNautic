#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML\Window\Event.hpp>

#include "Core/Io/Input.hpp"
#include "Core/Gui/HUD.hpp"
#include "Core/Audio/PlayerAudio.hpp"
#include "Core/World/Ship.hpp"
#include "GFX/Rendering/DeferredRenderer.hpp"
#include "Core/Utility/Camera.h"

class Player
{
public:
	Player(int id);
	~Player();
	Player(const Player& other);

	void update(float dt);
	void render(GFX::DeferredRenderer& renderer);
    const sf::Drawable& getHud() const;

	void setScreenSize(int screenWidth, int screenHeight, int offsetX, int offsetY);

	Ship& getShip()
	{
		return _ship;
	}

	Camera* getCamera();

	void setProgression(float progression);
	void setPosition(int position);

	bool _bIsFirstPerson;

	void swapPerspective();

private:
	int				_playerId;
	Input			_input;
	Ship			_ship;
	HUD				_hud;
	PlayerAudio		_audio;
	Camera			_camera;
	Camera			_fpCamera;
	Camera*			_currentCamera;
};

#endif // !PLAYER_HPP
