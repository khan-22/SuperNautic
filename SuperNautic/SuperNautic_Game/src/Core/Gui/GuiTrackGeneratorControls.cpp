
#include "Core/Gui/GuiTrackGeneratorControls.hpp"
#include "Core/Gui/GuiTrackGenerator.hpp"
#include "Core/Gui/GuiTextInput.hpp"
#include "Core/Gui/GuiSlider.hpp"
#include "Core/Gui/GuiHorizontalList.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Gui/GuiText.hpp"
#include "Core/Gui/GuiToolTip.hpp"

GuiTrackGeneratorControls::GuiTrackGeneratorControls(GuiTrackGenerator& generator, GuiToolTip* toolTip, Track* existingTrack)
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
    insertDifficultyInput(defaultPreset);
    insertRandSeedButton();
    insertShuffleButton();
    insertSaveButton();

    registerToolTips(toolTip);




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
        defaultPreset.difficulty = existingTrack->getDifficulty();
    }
    else
    {
        defaultPreset.seed = "AAAAA";
        defaultPreset.length = Track::_MIN_LENGTH / 1000;
        defaultPreset.curviness = Track::_MAX_CURVINESS;
        defaultPreset.difficulty = Track::_MIN_DIFFICULTY;
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

    GuiText* label = new GuiText("Seed", FontCache::get("res/arial.ttf"));

    label->setOrigin(label->getBoundingRect().width / 2.f, label->getBoundingRect().height);
    sf::FloatRect seedListBounds = _seedList->getBoundingRect();
    label->setPosition(seedListBounds.left + seedListBounds.width / 2.f, seedListBounds.top - label->getBoundingRect().height / 2.f);

    _seedList->registerOnSelect([label](){label->toggleSelection(); label->setFillColor(255, 255, 255);});
    _seedList->registerOnDeselect([label](){label->toggleSelection();});

    std::unique_ptr<SceneNode> labelPtr(label);
    _seedList->attachChild(labelPtr);


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
    static const size_t MIN_TRACK_LENGTH_KM = Track::_MIN_LENGTH / 1000;
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
    GuiText* label = new GuiText("Length  ", FontCache::get("res/arial.ttf"));

    label->setOrigin(label->getBoundingRect().width, label->getBoundingRect().height / 2.f);
    sf::FloatRect lengthBounds = _lengthInput->getBoundingRect();
    label->setPosition(lengthBounds.left, _lengthInput->getPosition().y + lengthBounds.height / 2.f);

    _lengthInput->registerOnSelect([label](){label->toggleSelection();});
    _lengthInput->registerOnDeselect([label](){label->toggleSelection();});

    std::unique_ptr<SceneNode> labelPtr(label);
    _lengthInput->attachChild(labelPtr);

    std::unique_ptr<GuiElement> lengthInputPtr(_lengthInput);
    insert(lengthInputPtr);
}

void GuiTrackGeneratorControls::insertCurvinessInput(const TrackPresetManager::Preset& defaultPreset)
{
    _curvinessInput = new GuiSlider
    (
        Track::_MIN_CURVINESS, Track::_MAX_CURVINESS,
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

void GuiTrackGeneratorControls::insertDifficultyInput(const TrackPresetManager::Preset& defaultPreset)
{
    _difficultyInput = new GuiSlider
    (
        Track::_MIN_DIFFICULTY, Track::_MAX_DIFFICULTY,
        50.f,
        6,
        sf::Text("Difficulty  Low", *_font.get()),
        sf::Text("High", *_font.get())
    );
    _difficultyInput->setValue(defaultPreset.difficulty);
    _difficultyInput->setOnChange([this](float c)
    {
        LOG("Difficulty: ", (int)c);
        _currentPreset->difficulty = c;
        _generator.setDifficulty(c);
        _generator.generate();
    });

    std::unique_ptr<GuiElement> difficultyInputPtr(_difficultyInput);
    insert(difficultyInputPtr);
}


void GuiTrackGeneratorControls::insertRandSeedButton()
{
    _randSeedButton = new GuiButton(sf::Text("Random seed", *_font.get()), [this]()
    {
        std::string seed = randString(_presetManager.getSeedLength());
        _selectedSeedInput->setText(seed);
        _currentPreset->seed = seed;
        _generator.setSeed(seed);
        _generator.generate();
    });
    std::unique_ptr<GuiElement> button(_randSeedButton);
    insert(button);
}

void GuiTrackGeneratorControls::insertShuffleButton()
{
    _shuffleButton = new GuiButton(sf::Text("Shuffle", *_font.get()), [&]()
    {
        std::string seed = randString(_presetManager.getSeedLength());
        _selectedSeedInput->setText(seed);
        _generator.setSeed(seed);

        size_t length = (Track::_MIN_LENGTH + rand() % ((Track::_MAX_LENGTH - Track::_MIN_LENGTH) / 2)) / 1000;
        std::string lengthText = std::to_string(length);
        if(lengthText.size() < _lengthInput->getText().size())
        {
            lengthText.insert(0, _lengthInput->getText().size() - lengthText.size(), '0');
        }
        _lengthInput->setText(lengthText);
        _generator.setLength(length * 1000);

        size_t curviness = Track::_MIN_CURVINESS + rand() % (Track::_MAX_CURVINESS - Track::_MIN_CURVINESS);
        _curvinessInput->setValue(curviness);
        _generator.setCurviness(curviness);

        size_t difficulty = Track::_MIN_DIFFICULTY + rand() % (Track::_MAX_DIFFICULTY - Track::_MIN_DIFFICULTY);
        _difficultyInput->setValue(difficulty);
        _generator.setDifficulty(difficulty);


        _currentPreset->seed = seed;
        _currentPreset->length = length;
        _currentPreset->curviness = curviness;
        _currentPreset->difficulty = difficulty;


        _generator.generate();
    });
    std::unique_ptr<GuiElement> button(_shuffleButton);
    insert(button);
}

void GuiTrackGeneratorControls::insertSaveButton()
{
    _saveButton = new GuiButton(sf::Text("Save", *_font.get()), [this]()
    {
        TrackPresetManager::Preset p;
        p.seed = _selectedSeedInput->getText();
        p.curviness = _curvinessInput->getValue();
        p.difficulty = _difficultyInput->getValue();
        std::stringstream sstream(_lengthInput->getText());
        sstream >> p.length;
        _presetManager.insert(p);

        insertSeedInput(&_presetManager.getPresets().back());
    });
    std::unique_ptr<GuiElement> button(_saveButton);
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
        _difficultyInput->setValue(preset->difficulty);
        _lengthInput->setText(toLengthInputString(preset->length));

        _generator.setSeed(preset->seed);
        _generator.setCurviness(preset->curviness);
        _generator.setDifficulty(preset->difficulty);
        _generator.setLength(preset->length * 1000);
        _generator.generate();
    });

    std::unique_ptr<GuiElement> seedInputPtr(seedInput);
    _seedList->insert(seedInputPtr);
}


void GuiTrackGeneratorControls::registerToolTips(GuiToolTip* toolTip)
{
    if(toolTip == nullptr)
    {
        return;
    }

    toolTip->registerTip(_lengthInput, "Length of the track in kilometres. Edit the length by pressing A.");
    toolTip->registerTip(_curvinessInput, "How much the track curves. A low curviness creates a straighter track. A high curviness creates a track with more curves.");
    toolTip->registerTip(_difficultyInput, "How many obstacles the track has. A low difficulty creates an easy track with few obstacles. A high difficulty creates a hard track with many obstacles.");
    toolTip->registerTip(_seedList, "Select a seed from which to generate the track. Edit the selected seed by pressing A.");
    toolTip->registerTip(_randSeedButton, "Randomize the selected seed.");
    toolTip->registerTip(_shuffleButton, "Randomize all parameters of the selected seed (including the seed itself).");
    toolTip->registerTip(_saveButton, "Save the current seed and parameters. When saved, it will appear at the end (to the right) in the seed list.");

}
