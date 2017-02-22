#pragma once

#ifndef WORLD_AUDIO_HPP
#define WORLD_AUDIO_HPP

#include <SFML\Audio.hpp>

class WorldAudio
{
public:
	WorldAudio();
	~WorldAudio();

	void play();
	void stop();
	void loadSound(std::string filename);
protected:

private:
	sf::SoundBuffer _musicbuffer;
	sf::Sound _music;

};

#endif // !WORLDAUDIO_H
