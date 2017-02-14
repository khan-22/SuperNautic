#include <string>

#include "WorldAudio.hpp"

WorldAudio::WorldAudio()
{
	std::string folder = "./res/audio/music/";
	if (!_music.openFromFile(folder + "Stranger Danger - Haste(Repeat).wav"))
	{
	}

	_music.setLoop(true);
	_music.play();
}

WorldAudio::~WorldAudio()
{
	_music.stop();
}

void WorldAudio::play() {
	_music.play();
}

void WorldAudio::stop() {
	_music.stop();
}