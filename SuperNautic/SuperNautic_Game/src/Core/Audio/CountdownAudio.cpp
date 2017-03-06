#include "Core/Audio/CountdownAudio.hpp"
#include "Core/Asset/AssetCache.hpp"

CountdownAudio::CountdownAudio() :
	_sbCountdown(AssetCache<sf::SoundBuffer, std::string>::get("counter"))
{
	setVolume(25);

	_sCountdown.setBuffer(*_sbCountdown.get());
}

CountdownAudio::~CountdownAudio()
{
}

void CountdownAudio::playCountdown()
{
	_sCountdown.setPlayingOffset(sf::milliseconds(0));
	_sCountdown.play();
}

void CountdownAudio::setVolume(float volume)
{
	_sCountdown.setVolume(volume);
}

bool CountdownAudio::isPlaying()
{
	if (_sCountdown.getStatus() == sf::Sound::Playing)
	{
		return true;
	}
	
	return false;
}