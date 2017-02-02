#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML\Window\Event.hpp>

#include "Input.hpp"
#include "Ship.hpp"
#include "HUD.hpp"
#include "PlayerAudio.hpp"

class Player
{
public:
	Player();
	Player(int id);
	~Player();

	void update(float dt);
	void render();
    const sf::Drawable& getHud() const;

private:
	int _playerId;
	Input _input;
	Ship _ship;
	HUD _hud;
	PlayerAudio _audio;

};

#endif // !PLAYER_HPP
