#include "WorldAudio.hpp"

WorldAudio::WorldAudio()
{
	if (!music.openFromFile(folder + "ariel" + fileSuffix))
	{
	}

	music.setLoop(true);
	music.play();
}

WorldAudio::~WorldAudio()
{
}