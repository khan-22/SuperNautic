#pragma once
#ifndef WORLDAUDIO_H

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

WorldAudio::WorldAudio()
{
	if (!music.openFromFile(folder + "ariel" + fileSuffix))
	{
	}

	music.setLoop(true);
	music.play();
}

WorldAudio::~WorldAudio()
{
}

#endif // !WORLDAUDIO_H
