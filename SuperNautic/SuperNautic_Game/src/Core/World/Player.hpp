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
	Player();
	Player(int id);
	~Player();
	Player(const Player& other);

	void update(float dt);
	void render(GFX::DeferredRenderer& renderer);
    const sf::Drawable& getHud() const;

	Ship& getShip()
	{
		return _ship;
	}

	const Camera* getCamera();

private:
	int							_playerId;
	Input						_input;
	Ship						_ship;
	HUD							_hud;
	PlayerAudio					_audio;
	Camera						_camera;

};

#endif // !PLAYER_HPP
