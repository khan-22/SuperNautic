#include "PlayerAudio.hpp"
#include "AssetCache.hpp"

PlayerAudio::PlayerAudio() :
	_sbVag(AssetCache<sf::SoundBuffer, std::string>::get("cat"))
{
	loadFromBuffers();
}

PlayerAudio::~PlayerAudio()
{

}

void PlayerAudio::playAudio(Sounds sound)
{
	switch (sound)
	{
	case vag:
		if (_sVag.getStatus() != _sVag.Playing)
		{
			_sVag.play();
		}
		break;
	default:
		break;
	}
}

void PlayerAudio::setPitch(Sounds sound, float pitchValue)
{
	switch (sound)
	{
	case vag:
		_sVag.setPitch(pitchValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::setVolume(Sounds sound, float volumeValue)
{
	switch (sound)
	{
	case vag:
		_sVag.setVolume(volumeValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::loadFromBuffers()
{
	_sVag.setBuffer(*_sbVag.get());
}
