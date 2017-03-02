
#include "Core/Gui/GuiTrackGeneratorControls.hpp"
#include "Core/Gui/GuiTrackGenerator.hpp"
#include "Core/Gui/GuiTextInput.hpp"
#include "Core/Gui/GuiSlider.hpp"
#include "Core/Gui/GuiHorizontalList.hpp"
#include "Core/Gui/GuiButton.hpp"

GuiTrackGeneratorControls::GuiTrackGeneratorControls(GuiTrackGenerator& generator, Track* existingTrack)
: GuiContainer()
, _generator(generator)
, _font(FontCache::get("res/arial.ttf"))
{
    TrackPresetManager::Preset defaultPreset = getDefaultPreset(existingTrack);

    _generator.setSeed(defaultPreset.seed);
    _generator.setLength(defaultPreset.length * 1000);
    _generator.setCurviness(defaultPreset.curviness);

    insertSeedList(defaultPreset);
    insertLengthInput(defaultPreset);
    insertCurvinessInput(defaultPreset);
    insertRandSeedButton();
    insertShuffleButton();
    insertSaveButton();


    // Make seedlist select the first seedinput element.
    _seedList->toggleSelection();
    _seedList->toggleSelection();



    sf::Vector2f pos(0.f, -_elements.front()->getBoundingRect().height);
    for(const std::unique_ptr<GuiElement>& e : _elements)
    {
        pos.y += e->getBoundingRect().height * 1.5f;
        e->setPosition(pos);
    }

    updateSize();
}


TrackPresetManager::Preset GuiTrackGeneratorControls::getDefaultPreset(Track* existingTrack)
{
    TrackPresetManager::Preset defaultPreset;
    if(existingTrack != nullptr)
    {
        defaultPreset.seed = existingTrack->getSeed();
        defaultPreset.length = existingTrack->getTargetLength() / 1000;
        defaultPreset.curviness = existingTrack->getCurviness();
    }
    else
    {
        defaultPreset.seed = "AAAAA";
        defaultPreset.length = 10;
        defaultPreset.curviness = 3;
    }

    return defaultPreset;
}



void GuiTrackGeneratorControls::insertSeedList(const TrackPresetManager::Preset& defaultPreset)
{
    _seedList = new GuiHorizontalList();


    _presets.assign(_presetManager.getPresets().begin(), _presetManager.getPresets().end());
    _presets.push_front(defaultPreset);
    for(const TrackPresetManager::Preset& p : _presets)
    {
        insertSeedInput(&p);
    }

    std::unique_ptr<GuiElement> seedListPtr(_seedList);
    insert(seedListPtr);
}

std::string GuiTrackGeneratorControls::toLengthInputString(unsigned int length) const
{
    std::string str = std::to_string(length);
    if(str.size() < _lengthInput->getText().size())
    {
        str.insert(0, _lengthInput->getText().size() - str.size(), '0');
    }
    return str;
}

void GuiTrackGeneratorControls::insertLengthInput(const TrackPresetManager::Preset& defaultPreset)
{
    static const size_t LENGTH_TEXT_LENGTH = 3;
    static const size_t MIN_TRACK_LENGTH_KM = Track::getMinLength() / 1000;
    _lengthInput = new GuiTextInput(LENGTH_TEXT_LENGTH, GuiCharacterInput::CharacterFlags::DIGITS);

    _lengthInput->setText(toLengthInputString(defaultPreset.length));
    _lengthInput->setOnChange([this](const std::string& str)
    {
        std::stringstream sstream(str);
        size_t c;
        sstream >> c;
        if(c < MIN_TRACK_LENGTH_KM)
        {
            c = MIN_TRACK_LENGTH_KM;
        }

        _currentPreset->length = c;
        _lengthInput->setText(toLengthInputString(c));
        _generator.setLength(c * 1000);
        _generator.generate();
    });
    _lengthInput->setOrigin(_lengthInput->getBoundingRect().width / 2.f, _lengthInput->getBoundingRect().height / 2.f);
    std::unique_ptr<GuiElement> lengthInputPtr(_lengthInput);
    insert(lengthInputPtr);
}

void GuiTrackGeneratorControls::insertCurvinessInput(const TrackPresetManager::Preset& defaultPreset)
{
    _curvinessInput = new GuiSlider
    (
        0.f, 5.f,
        50.f,
        6,
        sf::Text("Curviness  Low", *_font.get()),
        sf::Text("High", *_font.get())
    );
    _curvinessInput->setValue(defaultPreset.curviness);
    _curvinessInput->setOnChange([this](float c)
    {
        LOG("Curviness: ", (int)c);
        _currentPreset->curviness = c;
        _generator.setCurviness(c);
        _generator.generate();
    });

    std::unique_ptr<GuiElement> curvinessInputPtr(_curvinessInput);
    insert(curvinessInputPtr);
}


void GuiTrackGeneratorControls::insertRandSeedButton()
{
    std::unique_ptr<GuiElement> button(new GuiButton(sf::Text("Random seed", *_font.get()), [this]()
    {
        std::string seed = randString(5);
        _selectedSeedInput->setText(seed);
        _currentPreset->seed = seed;
        _generator.setSeed(seed);
        _generator.generate();
    }));
    insert(button);
}

void GuiTrackGeneratorControls::insertShuffleButton()
{
    std::unique_ptr<GuiElement> button(new GuiButton(sf::Text("Shuffle", *_font.get()), [&]()
    {
        std::string seed = randString(5);
        _selectedSeedInput->setText(seed);
        _generator.setSeed(seed);

        size_t length = Track::getMinLength() / 1000 + rand() % 50;
        std::string lengthText = std::to_string(length);
        if(lengthText.size() < _lengthInput->getText().size())
        {
            lengthText.insert(0, _lengthInput->getText().size() - lengthText.size(), '0');
        }
        _lengthInput->setText(lengthText);
        _generator.setLength(length * 1000);

        size_t curviness = rand() % 6;
        _curvinessInput->setValue(curviness);
        _generator.setCurviness(curviness);

        _currentPreset->seed = seed;
        _currentPreset->length = length;
        _currentPreset->curviness = curviness;

        _generator.generate();
    }));
    insert(button);
}

void GuiTrackGeneratorControls::insertSaveButton()
{
    std::unique_ptr<GuiElement> button(new GuiButton(sf::Text("Save", *_font.get()), [this]()
    {
        TrackPresetManager::Preset p;
        p.seed = _selectedSeedInput->getText();
        p.curviness = _curvinessInput->getValue();
        std::stringstream sstream(_lengthInput->getText());
        sstream >> p.length;
        _presetManager.insert(p);

        insertSeedInput(&_presetManager.getPresets().back());
    }));
    insert(button);
}

void GuiTrackGeneratorControls::insertSeedInput(const TrackPresetManager::Preset* preset)
{
    GuiTextInput* seedInput = new GuiTextInput(_presetManager.getSeedLength(), GuiCharacterInput::CharacterFlags::ALL);
    seedInput->setText(preset->seed);
    seedInput->setOnChange([this](const std::string& str)
    {
        LOG("Seed: \"", str, "\"");
        _generator.setSeed(str);
        _generator.generate();
    });
    seedInput->registerOnSelect([this, preset, seedInput]()
    {
        _selectedSeedInput = seedInput;
        _currentPreset = (TrackPresetManager::Preset*)preset;

        _curvinessInput->setValue(preset->curviness);
        _lengthInput->setText(toLengthInputString(preset->length));

        _generator.setSeed(preset->seed);
        _generator.setCurviness(preset->curviness);
        _generator.setLength(preset->length * 1000);
        _generator.generate();
    });

    std::unique_ptr<GuiElement> seedInputPtr(seedInput);
    _seedList->insert(seedInputPtr);
}
