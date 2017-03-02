#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/PreGameApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PlayApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"


#include "Core/Gui/GuiTextInput.hpp"
#include "Core/Gui/GuiHorizontalList.hpp"
#include "Core/Gui/GuiSlider.hpp"
#include "Core/Gui/GuiPlayerJoinContainer.hpp"

PreGameApplicationState::PreGameApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _trackGenerator(context.segmentHandler.get(), context.obstacleHandler.get())
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
, _numPlayers(0)
{
    std::string startSeed = "AAAAA";
    size_t startLength = 10000;
    size_t startCurviness = 3;

    if(_context.track != nullptr)
    {
        Track& t = *_context.track;
        startSeed = t.getSeed();
        startLength = t.getTargetLength();
        startCurviness = t.getCurviness();
    }


    GuiHorizontalList* seedList = new GuiHorizontalList();
//    seedList->setOnElementSelect([&](GuiElement* selection)
//    {
//        _selectedSeedInput = (GuiTextInput*)selection;
//        std::string text = _selectedSeedInput->getText();
//        LOG("Set new seed: \"", text, "\"");
//        _trackGenerator.setSeed(text);
//        _trackGenerator.generate();
//    });





    TrackPresetManager::Preset defaultPreset;
    defaultPreset.seed = startSeed;
    defaultPreset.length = startLength / 1000;
    defaultPreset.curviness = startCurviness;

    _trackGenerator.setSeed(defaultPreset.seed);
    _trackGenerator.setLength(defaultPreset.length * 1000);
    _trackGenerator.setCurviness(defaultPreset.curviness);

    std::vector<TrackPresetManager::Preset> presets = _presetManager.getPresets();
    presets.insert(presets.begin(), defaultPreset);
    for(const auto& p : presets)
    {
        _seedInputs.push_back(new GuiTextInput(_presetManager.getSeedLength(), GuiCharacterInput::CharacterFlags::ALL));
    }

    _selectedSeedInput = _seedInputs.front();
    auto onSeedChange = [this](const std::string& str)
    {
        LOG("Seed: \"", str, "\"");
        _trackGenerator.setSeed(str);
        _trackGenerator.generate();
    };

    assert(presets.size() == _seedInputs.size());
    for(size_t i = 0; i < presets.size(); i++)
    {
        GuiTextInput* seedInput = _seedInputs[i];
        TrackPresetManager::Preset p = presets[i];
        seedInput->setText(p.seed);
        seedInput->registerOnSelect([this, p, seedInput]()
        {
            _selectedSeedInput = seedInput;
            _curvinessInput->setValue(p.curviness);
            std::string lengthText = std::to_string(p.length);
            if(lengthText.size() < _lengthInput->getText().size())
            {
                lengthText.insert(0, _lengthInput->getText().size() - lengthText.size(), '0');
            }
            _lengthInput->setText(lengthText);

            _trackGenerator.setSeed(p.seed);
            _trackGenerator.setCurviness(p.curviness);
            _trackGenerator.setLength(p.length * 1000);
            _trackGenerator.generate();
        });

        seedInput->setOnChange(onSeedChange);

        auto seed = std::unique_ptr<GuiElement>(seedInput);
        seedList->insert(seed);
    }

    std::vector<std::unique_ptr<GuiElement>> guiElements;
    guiElements.emplace_back(seedList);

    static const size_t LENGTH_TEXT_LENGTH = 3;
    static const size_t MIN_TRACK_LENGTH_KM = 3;
    GuiTextInput* length = new GuiTextInput(LENGTH_TEXT_LENGTH, GuiCharacterInput::CharacterFlags::DIGITS);

    std::string lengthStr = std::to_string(startLength / 1000);
    if(lengthStr.size() < LENGTH_TEXT_LENGTH)
    {
        lengthStr.insert(0, LENGTH_TEXT_LENGTH - lengthStr.size(), '0');
    }
    length->setText(lengthStr);
    _lengthInput = length;
    length->setOnChange([this](const std::string& str)
    {
        std::stringstream sstream(str);
        size_t c;
        sstream >> c;
        if(c < MIN_TRACK_LENGTH_KM)
        {
            c = MIN_TRACK_LENGTH_KM;
        }
        std::string lengthText = std::to_string(c);
        if(lengthText.size() < LENGTH_TEXT_LENGTH)
        {
            lengthText.insert(0, LENGTH_TEXT_LENGTH - lengthText.size(), '0');
        }
        _lengthInput->setText(lengthText);
        _trackGenerator.setLength(c * 1000);
        _trackGenerator.generate();
    });
    length->setOrigin(length->getBoundingRect().width / 2.f, length->getBoundingRect().height / 2.f);

    guiElements.emplace_back(length);

    GuiSlider* curviness = new GuiSlider
    (
        0.f, 5.f,
        50.f,
        6,
        sf::Text("Curviness  Low", *_font.get()),
        sf::Text("High", *_font.get())
    );
    _curvinessInput = curviness;
    curviness->setValue(startCurviness);

    curviness->setOnChange([&](float c)
    {
        LOG("Curviness: ", (int)c);
        _trackGenerator.setCurviness(c);
        _trackGenerator.generate();
    });
    guiElements.emplace_back(curviness);

    GuiButton* randSeedButton = new GuiButton(sf::Text("Random seed", *_font.get()), [&]()
    {
        std::string seed = randString(5);
        _selectedSeedInput->setText(seed);
        _trackGenerator.setSeed(seed);
        _trackGenerator.generate();
    });
    guiElements.emplace_back(randSeedButton);

    GuiButton* shuffleButton = new GuiButton(sf::Text("Shuffle", *_font.get()), [&]()
    {
        std::string seed = randString(5);
        _selectedSeedInput->setText(seed);
        _trackGenerator.setSeed(seed);

        size_t length = MIN_TRACK_LENGTH_KM + rand() % 50;
        std::string lengthText = std::to_string(length);
        if(lengthText.size() < _lengthInput->getText().size())
        {
            lengthText.insert(0, _lengthInput->getText().size() - lengthText.size(), '0');
        }
        _lengthInput->setText(lengthText);
        _trackGenerator.setLength(length * 1000);

        size_t curviness = rand() % 6;
        _curvinessInput->setValue(curviness);
        _trackGenerator.setCurviness(curviness);

        _trackGenerator.generate();
    });
    guiElements.emplace_back(shuffleButton);


    GuiButton* startButton = new GuiButton(sf::Text("Start", *_font.get()), [&]()
    {
        _stack.clear();
        _context.numPlayers = _numPlayers == 0 ? 1 : _numPlayers;
        _context.track = _trackGenerator.takeTrack();
        _stack.push(std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(startButton);

    GuiButton* saveButton = new GuiButton(sf::Text("Save", *_font.get()), [this, seedList, onSeedChange]()
    {
        TrackPresetManager::Preset p;
        p.seed = _selectedSeedInput->getText();
        p.curviness = _curvinessInput->getValue();
        std::stringstream sstream(_lengthInput->getText());
        sstream >> p.length;
        _presetManager.insert(p);

        GuiTextInput* seedInput = new GuiTextInput(_presetManager.getSeedLength(), GuiCharacterInput::CharacterFlags::ALL);
        seedInput->setText(_presetManager.getPresets().back().seed);
        seedInput->setOnChange(onSeedChange);
        seedInput->registerOnSelect([this, p, seedInput]()
        {
            _selectedSeedInput = seedInput;
            _curvinessInput->setValue(p.curviness);

            std::string lengthText = std::to_string(p.length);
            if(lengthText.size() < LENGTH_TEXT_LENGTH)
            {
                lengthText.insert(0, LENGTH_TEXT_LENGTH - lengthText.size(), '0');
            }
            _lengthInput->setText(lengthText);

            _trackGenerator.setSeed(p.seed);
            _trackGenerator.setCurviness(p.curviness);
            _trackGenerator.setLength(p.length * 1000);
            _trackGenerator.generate();
        });

        _seedInputs.push_back(seedInput);

        auto seed = std::unique_ptr<GuiElement>(seedInput);
        seedList->insert(seed);
    });
    guiElements.emplace_back(saveButton);

    GuiButton* backButton = new GuiButton(sf::Text("Back", *_font.get()), [&]()
    {
        _stack.clear();
        _context.track = _trackGenerator.takeTrack();
        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
    });
    guiElements.emplace_back(backButton);

    GuiPlayerJoinContainer* players = new GuiPlayerJoinContainer();
    players->setOnJoin([this](unsigned char playerId)
    {
        _numPlayers++;
    });
    players->setOnLeave([this](unsigned char playerId)
    {
        _numPlayers--;
    });
    guiElements.emplace_back(players);

    sf::Vector2f pos(0.f, -guiElements.front()->getBoundingRect().height);
    for(const std::unique_ptr<GuiElement>& e : guiElements)
    {
        pos.y += e->getBoundingRect().height * 1.5f;
        e->setPosition(pos);
    }

    _guiContainer.setBackground(sf::Color(27, 173, 222, 100), sf::Color(19, 121, 156, 100), 5.f);
    _guiContainer.insert(guiElements);

    sf::Vector2u windowSize = _context.window.getSize();
    sf::FloatRect guiBounds = _guiContainer.getBoundingRect();
    _guiContainer.setOrigin(guiBounds.left + guiBounds.width / 2.f, guiBounds.top + guiBounds.height / 2.f);
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    _guiContainer.select(startButton);

    _forwardRenderer.initialize(&_context.window, 0.f, 0.f, 1.f, 1.f);

    _trackGenerator.generate();
}

void PreGameApplicationState::render()
{
    _forwardRenderer.render(_trackGenerator);
    static Camera garbageCam(glm::radians(90.f), 1280.f, 720.f);
    _forwardRenderer.display(garbageCam);

    _sfmlRenderer.render(_guiContainer);
    _sfmlRenderer.display(_context.window);
}

bool PreGameApplicationState::bUpdate(float dtSeconds)
{
    _guiContainer.update();
    _trackGenerator.update(dtSeconds);

    if(_input.checkActive())
    {
        _input.update();
        for(const sf::Event& e : _input.getEvents())
        {
            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::B:
                case sf::Keyboard::Escape:
                    if(!_guiContainer.bIsActive())
                    {
                        _stack.clear();
                        _context.track = _trackGenerator.takeTrack();
                        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
                    }
                    break;

                default:
                    break;
                }
            }
            _guiContainer.handleEvent(e);
        }
    }

    return true;
}

bool PreGameApplicationState::bHandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::B:
        case sf::Keyboard::Escape:
            if(!_guiContainer.bIsActive())
            {
                _stack.clear();
                _context.track = _trackGenerator.takeTrack();
                _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
            }
            break;

        default:
            break;
        }
    }
    _guiContainer.handleEvent(event);
    return true;
}
