#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML\Window\Event.hpp>

#include "Input.hpp"
#include "Ship.hpp"
#include "HUD.hpp"
#include "PlayerAudio.hpp"
#include "../GFX/DeferredRenderer.hpp"

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

private:
	int _playerId;
	Input _input;
	Ship _ship;
	HUD _hud;
	PlayerAudio _audio;

};

#endif // !PLAYER_HPP
