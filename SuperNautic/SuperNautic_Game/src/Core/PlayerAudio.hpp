#pragma once

#ifndef PLAYER_AUDIO_HPP
#define PLAYER_AUDIO_HPP

#include <SFML\Audio.hpp>

#include "Asset.hpp"

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	enum Sounds {vag, engine};

	void playAudio(Sounds sound);

	void setPitch(Sounds sound, float pitchValue);
	void setVolume(Sounds sound, float volumeValue);


protected:

private:
	sf::Sound _sVag;
	sf::Sound _sEngine;
	Asset<sf::SoundBuffer> _sbVag;
	Asset<sf::SoundBuffer> _sbEngine;

	void loadFromBuffers();

};

#endif // !PLAYERAUDIO_H
