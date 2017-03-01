#include "Core/Utility/TrackPresetManager.hpp"
#include "Core/Utility/FileUtility.hpp"
#include "Core/Utility/Utilities.hpp"

const char* const TrackPresetManager::_CONFIG_PATH = "tracks.cfg";
const size_t TrackPresetManager::_MAX_SEED_LENGTH = 5;
const size_t TrackPresetManager::_MIN_LENGTH = 3;
const size_t TrackPresetManager::_MAX_LENGTH = 1000;
const size_t TrackPresetManager::_MIN_CURVINESS = 0;
const size_t TrackPresetManager::_MAX_CURVINESS = 5;


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

        unsigned int length;
        if(!_config.get("length" + iStr, preset.length))
        {
            break;
        }

        unsigned int curviness;
        if(!_config.get("curviness" + iStr, preset.curviness))
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
    }

    _config.write(_CONFIG_PATH);
}

void TrackPresetManager::scrubPreset(Preset& preset)
{
    preset.seed.resize(_MAX_SEED_LENGTH);
    preset.curviness = clamp(preset.curviness, _MIN_CURVINESS, _MAX_CURVINESS);
    preset.length = clamp(preset.length, _MIN_LENGTH, _MAX_LENGTH);
}

size_t TrackPresetManager::getSeedLength()
{
    return _MAX_SEED_LENGTH;
}
