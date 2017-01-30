#include "WorldAudio.hpp"

WorldAudio::WorldAudio()
{
	if (!_music.openFromFile("ariel"))
	{
	}

	_music.setLoop(true);
	_music.play();
}

WorldAudio::~WorldAudio()
{
}