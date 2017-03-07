#include "Core/Audio/CountdownAudio.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Utility/AudioOptions.hpp"

CountdownAudio::CountdownAudio() :
	_sbCountdown(AssetCache<sf::SoundBuffer, std::string>::get("counter"))
{
	AudioOptions options;
	setVolume(options.getEffectsVolume());

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
	_sCountdown.setVolume(volume * 100.f);
}

void CountdownAudio::pause()
{
    _sCountdown.pause();
}

void CountdownAudio::resume()
{
    if(_sCountdown.getStatus() == sf::Sound::Paused)
    {
        _sCountdown.play();
    }
}

bool CountdownAudio::isPlaying()
{
	if (_sCountdown.getStatus() == sf::Sound::Playing)
	{
		return true;
	}

	return false;
}

float CountdownAudio::getTimeLeft() const
{
    return (_sbCountdown.get()->getDuration() - _sCountdown.getPlayingOffset()).asSeconds();
}
