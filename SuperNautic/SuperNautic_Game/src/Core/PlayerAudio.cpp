#include "PlayerAudio.h"

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
		if (sCat.getStatus() != sCat.Playing)
		{
			sCat.play();
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
		sCat.setPitch(pitchValue);
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
		sCat.setVolume(volumeValue);
		break;
	default:
		break;
	}
}

void PlayerAudio::loadFromBuffers()
{
	if (sbCat.loadFromFile(folder + "cat" + fileSuffix))
	{
		sCat.setBuffer(sbCat);
	}
}
