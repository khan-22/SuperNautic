#include <SFML/Audio/SoundBufferRecorder.hpp>
#include <string>

#include "Core/Asset/AssetCache.hpp"
#include "Core/Audio/WorldAudio.hpp"
#include "Core/Utility/AudioOptions.hpp"

WorldAudio::WorldAudio()
    : _sbCountdown(AssetCache<sf::SoundBuffer, std::string>::get("counter")) {
  _playlist.push_back("JJ - Mellow Business");
  _playlist.push_back("JJ - Haste");

  loadBuffers();

  _music.setBuffer(_musicbuffers.at(0));

  AudioOptions options;
  setVolume(options.getMusicVolume());

  _music.setLoop(true);
  playMusic();
}

WorldAudio::~WorldAudio() { _music.stop(); }

void WorldAudio::playMusic() {
  if (sf::SoundBufferRecorder::isAvailable()) {
    _music.play();
  }
}

void WorldAudio::stopMusic() { _music.stop(); }

void WorldAudio::loadBuffers() {
  std::string folder = "./res/audio/music/";
  for (int i = 0; i < _playlist.size(); i++) {
    _musicbuffers.push_back(sf::SoundBuffer());
    _musicbuffers[i].loadFromFile(folder + _playlist[i] + ".ogg");
  }
}

std::vector<std::string> WorldAudio::getPlaylist() { return _playlist; }

void WorldAudio::changeSong(std::string name) {
  int place = 0;

  for (int i = 0; i < _playlist.size(); i++) {
    if (_playlist[i] == name) {
      place = i;
      break;
    }
  }

  stopMusic();
  _music.setBuffer(_musicbuffers[place]);
  playMusic();
}

void WorldAudio::playCountdown() {
  _sCountdown.setPlayingOffset(sf::milliseconds(0));
  _sCountdown.play();
}

void WorldAudio::setVolume(float volume) {
  _music.setVolume(volume * 100.f);
  _sCountdown.setVolume(volume * 100.f);
}
