#pragma once

#ifndef COUNTDOWN_AUDIO_HPP
#define COUNTDOWN_AUDIO_HPP

#include <SFML/Audio.hpp>

#include <Core/Asset/Asset.hpp>

class CountdownAudio
{
public:
	CountdownAudio();
	~CountdownAudio();

	void playCountdown();

	// [0, 1]
	void setVolume(float volume);

	bool isPlaying();
protected:

private:
	sf::Sound _sCountdown;
	Asset<sf::SoundBuffer> _sbCountdown;
};

#endif // !COUNTDOWN_AUDIO_HPP
