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

	enum Sounds { goal, engine, overheat, shipCollision, obstacleCollision};

	void playAudio(Sounds sound);

	void setPitch(Sounds sound, float pitch);
	void setVolume(float volume);
	void setVolume(Sounds sound, float volume);

	void pause();
	void resume();

	void setRandomGoalBuffer(int pos);

protected:

private:
	sf::Sound _sGoal;
	sf::Sound _sEngine;
	sf::Sound _sOverheat;
	sf::Sound _sShipCollision;
	sf::Sound _sObstacleCollision;

	Asset<sf::SoundBuffer> _sbGoalYeah;
	Asset<sf::SoundBuffer> _sbGoalEasy;
	Asset<sf::SoundBuffer> _sbGoalRespect;
	Asset<sf::SoundBuffer> _sbGoalUnMe;
	Asset<sf::SoundBuffer> _sbGoalImprovising;
	Asset<sf::SoundBuffer> _sbGoalGYeah;
	Asset<sf::SoundBuffer> _sbEngine;
	Asset<sf::SoundBuffer> _sbOverheat;
	Asset<sf::SoundBuffer> _sbShipCollision;
	Asset<sf::SoundBuffer> _sbObstacleCollision;

	void loadFromBuffers();

};

#endif // !PLAYERAUDIO_H
