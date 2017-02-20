#include "Core/Audio/PlayerAudio.hpp"
#include "Core/Asset/AssetCache.hpp"

PlayerAudio::PlayerAudio() :
	_sbEngine(AssetCache<sf::SoundBuffer, std::string>::get("engine")),
	_sbCrash(AssetCache<sf::SoundBuffer, std::string>::get("crash")),
	_sbHit(AssetCache<sf::SoundBuffer, std::string>::get("hit")),
	_sbOverheat(AssetCache<sf::SoundBuffer, std::string>::get("overheat"))
{
	loadFromBuffers();

	_sEngine.setLoop(true);

	playAudio(Sounds::engine);
}

PlayerAudio::~PlayerAudio()
{

}

void PlayerAudio::playAudio(Sounds sound)
{
	switch (sound)
	{
	case engine:
		if (_sEngine.getStatus() != _sEngine.Playing)
		{
			_sEngine.play();
		}
		break;
	case crash:
		if (_sCrash.getStatus() != _sCrash.Playing)
		{
			_sCrash.play();
		}
		break;
	case hit:
		if (_sHit.getStatus() != _sHit.Playing)
		{
			_sHit.play();
		}
		break;
	case overheat:
		if (_sOverheat.getStatus() != _sOverheat.Playing)
		{
			_sOverheat.play();
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
	case engine:
		_sEngine.setPitch(pitchValue);
		break;
	case crash:
		_sCrash.setPitch(pitchValue);
		break;
	case hit:
		_sHit.setPitch(pitchValue);
		break;
	case overheat:
		_sOverheat.setPitch(pitchValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::setVolume(Sounds sound, float volumeValue)
{
	switch (sound)
	{
	case engine:
		_sEngine.setVolume(volumeValue);
		break;
	case crash:
		_sCrash.setVolume(volumeValue);
		break;
	case hit:
		_sHit.setVolume(volumeValue);
		break;
	case overheat:
		_sOverheat.setVolume(volumeValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::loadFromBuffers()
{
	_sEngine.setBuffer(*_sbEngine.get());
	_sCrash.setBuffer(*_sbCrash.get());
	_sHit.setBuffer(*_sbHit.get());
	_sOverheat.setBuffer(*_sbOverheat.get());
}
