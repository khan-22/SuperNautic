#pragma once

#ifndef WORLD_AUDIO_HPP
#define WORLD_AUDIO_HPP

#include <SFML/Audio.hpp>

#include <Core/Asset/Asset.hpp>

class WorldAudio
{
public:
	WorldAudio();
	~WorldAudio();

	void playMusic();
	void stopMusic();
	void loadSound(std::string filename);


	void playCountdown();

	// [0, 1]
	void setVolume(float volume);
protected:

private:
	sf::SoundBuffer _musicbuffer;
	sf::Sound		_music;

	sf::Sound _sCountdown;
	Asset<sf::SoundBuffer> _sbCountdown;
};

#endif // !WORLDAUDIO_H
