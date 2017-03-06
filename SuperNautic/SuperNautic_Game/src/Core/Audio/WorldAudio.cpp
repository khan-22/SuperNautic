#include <string>
#include <SFML/Audio/SoundBufferRecorder.hpp>

#include "Core/Audio/WorldAudio.hpp"
#include "Core/Utility/AudioOptions.hpp"

WorldAudio::WorldAudio()
{
	loadSound("Stranger Danger - Mellow Business.ogg");
	_music.setBuffer(_musicbuffer);


	AudioOptions options;
	setVolume(options.getMusicVolume());
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

void WorldAudio::loadSound(std::string filename)
{
	std::string folder = "./res/audio/music/";
	_musicbuffer.loadFromFile(folder + filename);
}

void WorldAudio::setVolume(float volume)
{
    _music.setVolume(volume * 100.f);
}
