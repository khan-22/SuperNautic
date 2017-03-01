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
    };

    TrackPresetManager();

    void insert(const Preset& preset);

    const std::vector<Preset>& getPresets() const;

    static size_t getSeedLength();

private:
    static const char* const _CONFIG_PATH;
    static const size_t _MAX_SEED_LENGTH;
    static const size_t _MIN_LENGTH;
    static const size_t _MAX_LENGTH;
    static const size_t _MIN_CURVINESS;
    static const size_t _MAX_CURVINESS;

    ConfigFile  _config;
    std::vector<Preset> _presets;

    void readConfig();
    void writeConfig();

    static void scrubPreset(Preset& preset);
};


#endif //TRACK_PRESET_MANAGER_HPP
