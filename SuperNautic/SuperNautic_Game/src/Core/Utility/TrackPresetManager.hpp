#pragma once

#ifndef TRACK_PRESET_MANAGER_HPP
#define TRACK_PRESET_MANAGER_HPP

#include <vector>

#include "Core/Io/ConfigFile.hpp"

class TrackPresetManager
{
public:
    struct Preset
    {
        std::string seed;
        unsigned int length; // in kilometres
        unsigned int curviness;
        unsigned int difficulty;
    };

    TrackPresetManager();

    void insert(const Preset& preset);

    const std::vector<Preset>& getPresets() const;

    static size_t getSeedLength();

private:
    static const char* const _CONFIG_PATH;
    static const size_t _MAX_SEED_LENGTH;

    ConfigFile  _config;
    std::vector<Preset> _presets;

    void readConfig();
    void writeConfig();

    static void scrubPreset(Preset& preset);
};


#endif //TRACK_PRESET_MANAGER_HPP
