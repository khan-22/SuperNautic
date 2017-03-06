#include <string>
#include <SFML/Audio/SoundBufferRecorder.hpp>

#include "Core/Audio/WorldAudio.hpp"
#include "Core/Asset/AssetCache.hpp"

WorldAudio::WorldAudio() :
	_sbCountdown(AssetCache<sf::SoundBuffer, std::string>::get("counter"))
{
	loadSound("Stranger Danger - Mellow Business.ogg");
	_music.setBuffer(_musicbuffer);

	setVolume(50);

	_music.setLoop(true);
	playMusic();
}

WorldAudio::~WorldAudio()
{
	_music.stop();
}

void WorldAudio::playMusic() {
	if (sf::SoundBufferRecorder::isAvailable())
	{
		_music.play();
	}
}

void WorldAudio::stopMusic() {
	_music.stop();
}

void WorldAudio::loadSound(std::string filename)
{
	std::string folder = "./res/audio/music/";
	_musicbuffer.loadFromFile(folder + filename);
}

void WorldAudio::playCountdown()
{
	_sCountdown.setPlayingOffset(sf::milliseconds(0));
	_sCountdown.play();
}

void WorldAudio::setVolume(float volume)
{
	_music.setVolume(volume);
	_sCountdown.setVolume(volume);
}
