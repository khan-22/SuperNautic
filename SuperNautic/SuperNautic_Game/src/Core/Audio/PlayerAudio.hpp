#pragma once

#ifndef PLAYER_AUDIO_HPP
#define PLAYER_AUDIO_HPP

#include <SFML/Audio.hpp>

#include "Core/Asset/Asset.hpp"

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	enum Sounds {engine, overheat, shipCollision, obstacleCollision};

	void playAudio(Sounds sound);

	void setPitch(Sounds sound, float pitch);
	void setVolume(float volume);
	void setVolume(Sounds sound, float volume);

	void pause();
	void resume();

protected:

private:
	sf::Sound _sEngine;
	sf::Sound _sOverheat;
	sf::Sound _sShipCollision;
	sf::Sound _sObstacleCollision;
	Asset<sf::SoundBuffer> _sbEngine;
	Asset<sf::SoundBuffer> _sbOverheat;
	Asset<sf::SoundBuffer> _sbShipCollision;
	Asset<sf::SoundBuffer> _sbObstacleCollision;

	void loadFromBuffers();

};

#endif // !PLAYERAUDIO_H
