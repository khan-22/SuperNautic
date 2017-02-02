#include "PlayerAudio.hpp"
#include "AssetCache.hpp"

PlayerAudio::PlayerAudio() :
	_sbVag(AssetCache<sf::SoundBuffer, std::string>::get("cat")),
	_sbEngine(AssetCache<sf::SoundBuffer, std::string>::get("engine"))
{
	loadFromBuffers();

	_sEngine.setLoop(true);
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
	case engine:
		if (_sEngine.getStatus() != _sEngine.Playing)
		{
			_sEngine.play();
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
	case engine:
		_sEngine.setPitch(pitchValue);
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
	case engine:
		_sEngine.setVolume(volumeValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::loadFromBuffers()
{
	_sVag.setBuffer(*_sbVag.get());
	_sEngine.setBuffer(*_sbEngine.get());
}
