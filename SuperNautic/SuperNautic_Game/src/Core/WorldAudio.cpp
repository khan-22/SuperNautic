#include "WorldAudio.hpp"

WorldAudio::WorldAudio()
{
	if (!_music.openFromFile(_folder + "ariel" + _fileSuffix))
	{
	}

	_music.setLoop(true);
	_music.play();
}

WorldAudio::~WorldAudio()
{
}