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
protected:

private:
	sf::Sound _buttonClick;
	sf::Music _music;

};

#endif // !WORLDAUDIO_H
