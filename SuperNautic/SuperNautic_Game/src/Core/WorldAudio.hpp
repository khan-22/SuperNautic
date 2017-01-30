#pragma once

#ifndef WORLD_AUDIO_HPP
#define WORLD_AUDIO_HPP

#include <SFML\Audio.hpp>

class WorldAudio
{
public:
	WorldAudio();
	~WorldAudio();

protected:

private:
	std::string _folder = "res/audio/", _fileSuffix = ".wav";

	sf::Sound _buttonClick;
	sf::Music _music;

};

#endif // !WORLDAUDIO_H
