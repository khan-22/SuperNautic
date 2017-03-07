#include "Core/Utility/AudioOptions.hpp"
#include "Core/Utility/FileUtility.hpp"
#include "Core/Utility/Utilities.hpp"

const char* const AudioOptions::_CONFIG_PATH = "config.cfg";
const float AudioOptions::_DEFAULT_MUSIC_VOLUME = 1.f;
const float AudioOptions::_DEFAULT_EFFECTS_VOLUME = 1.f;

AudioOptions::AudioOptions()
: _musicVolume(_DEFAULT_MUSIC_VOLUME)
, _effectsVolume(_DEFAULT_EFFECTS_VOLUME)
{
    readConfig();
}


void AudioOptions::readConfig()
{
    createFileIfNotExists(_CONFIG_PATH);
    _config.read(_CONFIG_PATH);

    float music;
    float effects;
    if
    (
        !_config.get("music", music) ||
        !_config.get("effects", effects)
    )
    {
        rebuildConfig();
        readConfig();
        return;
    }

    setMusicVolume(music);
    setEffectsVolume(effects);
}

void AudioOptions::rebuildConfig()
{
    _config.set("music", _DEFAULT_MUSIC_VOLUME);
    _config.set("effects", _DEFAULT_EFFECTS_VOLUME);
    _config.write(_CONFIG_PATH);
}

void AudioOptions::writeConfig()
{
    _config.set("music", _musicVolume);
    _config.set("effects", _effectsVolume);
    _config.write(_CONFIG_PATH);
}

void AudioOptions::setMusicVolume(float volume)
{
    _musicVolume = clamp(volume, 0.f, 1.f);
    writeConfig();
}

void AudioOptions::setEffectsVolume(float volume)
{
    _effectsVolume = clamp(volume, 0.f, 1.f);
    writeConfig();
}

float AudioOptions::getMusicVolume() const
{
    return _musicVolume;
}

float AudioOptions::getEffectsVolume() const
{
    return _effectsVolume;
}
