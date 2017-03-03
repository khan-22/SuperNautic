#pragma once

#ifndef GUI_TRACK_GENERATOR_CONTROLS_HPP
#define GUI_TRACK_GENERATOR_CONTROLS_HPP


#include "Core/Gui/GuiContainer.hpp"
#include "Core/Utility/TrackPresetManager.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"

class GuiTrackGenerator;
class Track;
class GuiTextInput;
class GuiSlider;
class GuiHorizontalList;

class GuiTrackGeneratorControls : public GuiContainer
{
public:
   GuiTrackGeneratorControls(GuiTrackGenerator& generator, Track* existingTrack = nullptr);

private:
    GuiTrackGenerator&  _generator;
    FontAsset           _font;
    TrackPresetManager  _presetManager;
    GuiTextInput*       _selectedSeedInput;
    GuiTextInput*       _lengthInput;
    GuiSlider*          _curvinessInput;
    GuiSlider*          _difficultyInput;
    GuiHorizontalList*  _seedList;
    std::list<TrackPresetManager::Preset> _presets;
    TrackPresetManager::Preset* _currentPreset;

    void insertSeedList(const TrackPresetManager::Preset& defaultPreset);
    void insertSeedInput(const TrackPresetManager::Preset* preset);

    void insertLengthInput(const TrackPresetManager::Preset& defaultPreset);
    void insertCurvinessInput(const TrackPresetManager::Preset& defaultPreset);
    void insertDifficultyInput(const TrackPresetManager::Preset& defaultPreset);
    void insertRandSeedButton();
    void insertShuffleButton();
    void insertSaveButton();

    std::string toLengthInputString(unsigned int length) const;

    static TrackPresetManager::Preset getDefaultPreset(Track* existingTrack);
};


#endif //GUI_TRACK_GENERATOR_CONTROLS_HPP
