#pragma once

#ifndef AUDIO_OPTIONS_HPP
#define AUDIO_OPTIONS_HPP

#include "Core/Io/ConfigFile.hpp"

class AudioOptions
{
public:
    AudioOptions();

    // [0, 1]
    void setMusicVolume(float volume);

    // [0, 1]
    void setEffectsVolume(float volume);

    float getMusicVolume() const;
    float getEffectsVolume() const;

private:
    static const char* const _CONFIG_PATH;
    static const float _DEFAULT_MUSIC_VOLUME;
    static const float _DEFAULT_EFFECTS_VOLUME;

    ConfigFile  _config;

    float _musicVolume;
    float _effectsVolume;


    void readConfig();
    void rebuildConfig();
    void writeConfig();
};


#endif //AUDIO_OPTIONS_HPP
