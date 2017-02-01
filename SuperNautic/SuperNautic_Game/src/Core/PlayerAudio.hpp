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

	enum Sounds {vag};

	void playAudio(Sounds sound);

	void setPitch(Sounds sound, float pitchValue);
	void setVolume(Sounds sound, float volumeValue);


protected:

private:
	sf::Sound _sVag;
	Asset<sf::SoundBuffer> _sbVag;

	void loadFromBuffers();

};

#endif // !PLAYERAUDIO_H
