#include "AudioHandler.h"

AudioHandler::AudioHandler()
{
	loadBuffers();
}

AudioHandler::~AudioHandler()
{
}

sf::SoundBuffer AudioHandler::getSoundBuffer(BufferName bn)
{
	switch (bn)
	{
	case AudioHandler::cat:
		return sbCat;
	default:
		break;
	}
}

int AudioHandler::loadBuffers()
{
	if (!sbCat.loadFromFile(folder + "cat" + fileSuffix))
	{
		return 0;
	}
	return 1;
}