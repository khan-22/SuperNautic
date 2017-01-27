#pragma once

#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <SFML\Audio.hpp>
#include <string.h>

class AudioHandler
{
public:
	AudioHandler();
	~AudioHandler();

	enum BufferName { cat };

	sf::SoundBuffer getSoundBuffer(BufferName bn);
	
protected:

private:
	std::string folder = "res/audio/", fileSuffix = ".wav";
	sf::SoundBuffer sbCat;

	int loadBuffers();
};

#endif // !AUDIOHANDLER_H
