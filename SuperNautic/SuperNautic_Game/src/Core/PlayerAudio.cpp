#include "PlayerAudio.hpp"

PlayerAudio::PlayerAudio()
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
	case cat:
		if (_sCat.getStatus() != _sCat.Playing)
		{
			_sCat.play();
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
	case cat:
		_sCat.setPitch(pitchValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::setVolume(Sounds sound, float volumeValue)
{
	switch (sound)
	{
	case cat:
		_sCat.setVolume(volumeValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::loadFromBuffers()
{
	if (_sbCat.loadFromFile(_folder + "cat" + _fileSuffix))
	{
		_sCat.setBuffer(_sbCat);
	}
}
