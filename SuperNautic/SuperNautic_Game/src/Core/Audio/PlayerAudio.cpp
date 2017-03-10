#include "Core/Audio/PlayerAudio.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Utility/AudioOptions.hpp"

#include <algorithm>
#include <stdlib.h>

PlayerAudio::PlayerAudio() :
	_sbEngine(AssetCache<sf::SoundBuffer, std::string>::get("engine")),
	_sbShipCollision(AssetCache<sf::SoundBuffer, std::string>::get("shipCollision")),
	_sbObstacleCollision(AssetCache<sf::SoundBuffer, std::string>::get("obstacleCollision")),
	_sbOverheat(AssetCache<sf::SoundBuffer, std::string>::get("overheat")),
	_sbGoalYeah(AssetCache<sf::SoundBuffer, std::string>::get("goalYeah")),
	_sbGoalEasy(AssetCache<sf::SoundBuffer, std::string>::get("goalEasy")),
	_sbGoalRespect(AssetCache<sf::SoundBuffer, std::string>::get("goalRespect")),
	_sbGoalImprovising(AssetCache<sf::SoundBuffer, std::string>::get("goalImprovising")),
	_sbGoalGYeah(AssetCache<sf::SoundBuffer, std::string>::get("goalGYeah")),
	_sbGoalUnMe(AssetCache<sf::SoundBuffer, std::string>::get("goalUnMe"))
{
	loadFromBuffers();

	_sEngine.setLoop(true);

	AudioOptions options;
	setVolume(options.getEffectsVolume() * 100.f);
	playAudio(Sounds::engine);
}

PlayerAudio::~PlayerAudio()
{

}

void PlayerAudio::pause()
{
	_sGoal.pause();
    _sEngine.pause();
	_sOverheat.pause();
    _sShipCollision.pause();
    _sObstacleCollision.pause();
}

void PlayerAudio::resume()
{
	if (_sGoal.getStatus() == sf::Sound::Paused)
	{
		_sGoal.play();
	}
    if(_sEngine.getStatus() == sf::Sound::Paused)
    {
        _sEngine.play();
    }
	if (_sOverheat.getStatus() == sf::Sound::Paused)
	{
		_sOverheat.play();
	}
    if(_sShipCollision.getStatus() == sf::Sound::Paused)
    {
        _sShipCollision.play();
    }
    if(_sObstacleCollision.getStatus() == sf::Sound::Paused)
    {
        _sObstacleCollision.play();
    }
}

void PlayerAudio::playAudio(Sounds sound)
{
	switch (sound)
	{
	case goal:
		_sGoal.setPlayingOffset(sf::milliseconds(0));
		_sGoal.play();
		break;
	case engine:
		_sEngine.setPlayingOffset(sf::milliseconds(0));
		_sEngine.play();
		break;
	case overheat:
		_sOverheat.setPlayingOffset(sf::milliseconds(0));
		_sOverheat.play();
		break;
	case shipCollision:
		_sShipCollision.setPlayingOffset(sf::milliseconds(0));
		_sShipCollision.play();
		break;
	case obstacleCollision:
		_sObstacleCollision.setPlayingOffset(sf::milliseconds(0));
		_sObstacleCollision.play();
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
	case goal:
		_sGoal.setPitch(pitchValue);
		break;
	case engine:
		_sEngine.setPitch(pitchValue);
		break;
	case overheat:
		_sOverheat.setPitch(pitchValue);
		break;
	case shipCollision:
		_sShipCollision.setPitch(pitchValue);
		break;
	case obstacleCollision:
		_sObstacleCollision.setPitch(pitchValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::setVolume(float volume)
{
	_sGoal.setVolume(volume);
	_sEngine.setVolume(volume);
	_sOverheat.setVolume(volume);
	_sShipCollision.setVolume(volume);
	_sObstacleCollision.setVolume(volume);
}

void PlayerAudio::setVolume(Sounds sound, float volume)
{
	switch (sound)
	{
	case goal:
		_sGoal.setVolume(volume);
		break;
	case engine:
		_sEngine.setVolume(volume);
		break;
	case overheat:
		_sOverheat.setVolume(volume);
		break;
	case shipCollision:
		_sShipCollision.setVolume(volume);
		break;
	case obstacleCollision:
		_sObstacleCollision.setVolume(volume);
		break;
	default:
		break;
	}
}

void PlayerAudio::loadFromBuffers()
{
	_sGoal.setBuffer(*_sbGoalYeah.get());
	_sEngine.setBuffer(*_sbEngine.get());
	_sOverheat.setBuffer(*_sbOverheat.get());
	_sShipCollision.setBuffer(*_sbShipCollision.get());
	_sObstacleCollision.setBuffer(*_sbObstacleCollision.get());
}

void PlayerAudio::setRandomGoalBuffer(int pos)
{
	if (pos == 1)
	{
		switch (rand() % 3)
		{
		case 0:
			_sGoal.setBuffer(*_sbGoalYeah.get());
			break;
		case 1:
			_sGoal.setBuffer(*_sbGoalEasy.get());
			break;
		case 2:
			_sGoal.setBuffer(*_sbGoalRespect.get());
			break;
		default:
			break;
		}
	}
	else
	{
		switch (rand() % 3)
		{
		case 0:
			_sGoal.setBuffer(*_sbGoalUnMe.get());
			break;
		case 1:
			_sGoal.setBuffer(*_sbGoalImprovising.get());
			break;
		case 2:
			_sGoal.setBuffer(*_sbGoalGYeah.get());
			break;
		default:
			break;
		}
	}
}