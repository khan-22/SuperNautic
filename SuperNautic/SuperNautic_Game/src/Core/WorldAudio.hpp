#pragma once
#ifndef WORLDAUDIO_HPP
#define WORLDAUDIO_HPP

#include <SFML\Audio.hpp>

class WorldAudio
{
public:
	WorldAudio();
	~WorldAudio();

protected:

private:
	std::string folder = "res/audio/", fileSuffix = ".wav";

	sf::Sound buttonClick;
	sf::Music music;

};

#endif // !WORLDAUDIO_H
