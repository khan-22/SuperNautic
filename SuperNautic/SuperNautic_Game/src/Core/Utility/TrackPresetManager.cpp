#include "Core/Utility/TrackPresetManager.hpp"
#include "Core/Track/Track.hpp"
#include "Core/Utility/FileUtility.hpp"
#include "Core/Utility/Utilities.hpp"

const char* const TrackPresetManager::_CONFIG_PATH = "tracks.cfg";
const size_t TrackPresetManager::_MAX_SEED_LENGTH = 5;


TrackPresetManager::TrackPresetManager()
{
    readConfig();
}

void TrackPresetManager::insert(const Preset& preset)
{
    _presets.push_back(preset);
    scrubPreset(_presets.back());

    writeConfig();
}

const std::vector<TrackPresetManager::Preset>& TrackPresetManager::getPresets() const
{
    return _presets;
}



void TrackPresetManager::readConfig()
{
    createFileIfNotExists(_CONFIG_PATH);

    _config.read(_CONFIG_PATH);

    for(size_t i = 0;; i++)
    {
        std::string iStr = std::to_string(i);
        Preset preset;
        if(!_config.get("seed" + iStr, preset.seed))
        {
            break;
        }

        if(!_config.get("length" + iStr, preset.length))
        {
            break;
        }

        if(!_config.get("curviness" + iStr, preset.curviness))
        {
            break;
        }

        if(!_config.get("difficulty" + iStr, preset.difficulty))
        {
            break;
        }

        scrubPreset(preset);
        _presets.push_back(preset);
    }

    writeConfig();
}

void TrackPresetManager::writeConfig()
{
    std::remove(_CONFIG_PATH);
    createFileIfNotExists(_CONFIG_PATH);

    _config.read(_CONFIG_PATH);

    for(size_t i = 0; i < _presets.size(); i++)
    {
        std::string iStr = std::to_string(i);
        const Preset& p = _presets[i];
        _config.set("seed" + iStr, p.seed);
        _config.set("length" + iStr, p.length);
        _config.set("curviness" + iStr, p.curviness);
        _config.set("difficulty" + iStr, p.difficulty);
    }

    _config.write(_CONFIG_PATH);
}

void TrackPresetManager::scrubPreset(Preset& preset)
{
    preset.seed.resize(_MAX_SEED_LENGTH);
    preset.curviness = clamp(preset.curviness, Track::_MIN_CURVINESS, Track::_MAX_CURVINESS);
    preset.length = clamp(preset.length, Track::_MIN_LENGTH / 1000, Track::_MAX_LENGTH / 1000);
    preset.difficulty = clamp(preset.difficulty, Track::_MIN_DIFFICULTY, Track::_MAX_DIFFICULTY);
}

size_t TrackPresetManager::getSeedLength()
{
    return _MAX_SEED_LENGTH;
}
