#pragma once

#ifndef PLAYERAUDIO_HPP
#define PLAYERAUDIO_HPP

#include <SFML\Audio.hpp>
#include <string.h>

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	enum Sounds {cat};

	void playAudio(Sounds sound);

	void setPitch(Sounds sound, float pitchValue);
	void setVolume(Sounds sound, float volumeValue);


protected:

private:
	std::string folder = "res/audio/", fileSuffix = ".wav";
	sf::Sound sCat;
	sf::SoundBuffer sbCat;

	void loadFromBuffers();

};

#endif // !PLAYERAUDIO_H
