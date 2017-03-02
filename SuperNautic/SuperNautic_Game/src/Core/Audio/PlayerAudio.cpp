#include "Core/Audio/PlayerAudio.hpp"
#include "Core/Asset/AssetCache.hpp"

#include <algorithm>

PlayerAudio::PlayerAudio() :
	_sbEngine(AssetCache<sf::SoundBuffer, std::string>::get("engine")),
	_sbCrash(AssetCache<sf::SoundBuffer, std::string>::get("crash")),
	_sbObstacleCollision(AssetCache<sf::SoundBuffer, std::string>::get("hit")),
	_sbOverheat(AssetCache<sf::SoundBuffer, std::string>::get("overheat"))
{
	loadFromBuffers();

	_sEngine.setLoop(true);

	setVolume(Sounds::crash, 80);
	setVolume(Sounds::engine, 80);
	setVolume(Sounds::obstacleCollision, 80);
	setVolume(Sounds::overheat, 80);

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
		_sEngine.setPlayingOffset(sf::milliseconds(0));
		_sEngine.play();
		break;
	case crash:
		_sCrash.setPlayingOffset(sf::milliseconds(0));
		_sCrash.play();
		break;
	case obstacleCollision:
		_sObstacleCollision.setPlayingOffset(sf::milliseconds(0));
		_sObstacleCollision.play();
		break;
	case overheat:
		_sOverheat.setPlayingOffset(sf::milliseconds(0));
		_sOverheat.play();
		break;
	default:
		break;
	}
}

void PlayerAudio::setPitch(Sounds sound, float pitchValue)
{
	float val = std::min(0.8f, std::max(2.f, pitchValue));
	switch (sound)
	{
	case engine:
		_sEngine.setPitch(pitchValue);
		break;
	case crash:
		_sCrash.setPitch(pitchValue);
		break;
	case obstacleCollision:
		_sObstacleCollision.setPitch(pitchValue);
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
	case obstacleCollision:
		_sObstacleCollision.setVolume(volumeValue);
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
	_sObstacleCollision.setBuffer(*_sbObstacleCollision.get());
	_sOverheat.setBuffer(*_sbOverheat.get());
}
