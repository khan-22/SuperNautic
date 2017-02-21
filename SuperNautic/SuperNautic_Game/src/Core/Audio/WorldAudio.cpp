#include <string>
#include <SFML/Audio/SoundBufferRecorder.hpp>

#include "Core/Audio/WorldAudio.hpp"

WorldAudio::WorldAudio()
{
	std::string folder = "./res/audio/music/";
	_music.openFromFile(folder + "Stranger Danger - Haste(Repeat).ogg");

	_music.setLoop(true);
	play();
}

WorldAudio::~WorldAudio()
{
	_music.stop();
}

void WorldAudio::play() {
	if (sf::SoundBufferRecorder::isAvailable())
	{
		_music.play();
	}
}

void WorldAudio::stop() {
	_music.stop();
}
