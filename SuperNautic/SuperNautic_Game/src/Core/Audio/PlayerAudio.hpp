#pragma once

#ifndef PLAYER_AUDIO_HPP
#define PLAYER_AUDIO_HPP

#include <SFML\Audio.hpp>

#include "Core/Asset/Asset.hpp"

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	enum Sounds {engine, overheat, crash, hit};

	void playAudio(Sounds sound);

	void setPitch(Sounds sound, float pitchValue);
	void setVolume(Sounds sound, float volumeValue);


protected:

private:
	sf::Sound _sEngine;
	sf::Sound _sOverheat;
	sf::Sound _sCrash;
	sf::Sound _sHit;
	Asset<sf::SoundBuffer> _sbEngine;
	Asset<sf::SoundBuffer> _sbOverheat;
	Asset<sf::SoundBuffer> _sbCrash;
	Asset<sf::SoundBuffer> _sbHit;

	void loadFromBuffers();

};

#endif // !PLAYERAUDIO_H
