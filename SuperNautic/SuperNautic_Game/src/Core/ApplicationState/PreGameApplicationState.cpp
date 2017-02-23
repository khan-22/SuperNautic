#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/PreGameApplicationState.hpp"
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
    GuiHorizontalList* seedList = new GuiHorizontalList();
    seedList->setOnElementSelect([&](GuiElement* selection)
    {
        _selectedSeedInput = (GuiTextInput*)selection;
        std::string text = _selectedSeedInput->getText();
        LOG("Set new seed: \"", text, "\"");
        _trackGenerator.setSeed(text);
        _trackGenerator.generate();
    });

    for(size_t i = 0; i < 10; i++)
    {
        GuiTextInput* seed = new GuiTextInput(5, GuiCharacterInput::CharacterFlags::ALL);
        _seedInputs.push_back(seed);
        seed->setText(std::string(5, 'A' + i));
        seed->setOnChange([this](const std::string& str)
        {
            LOG("Seed: \"", str, "\"");
            _trackGenerator.setSeed(str);
            _trackGenerator.generate();
        });
        auto seedPtr = std::unique_ptr<GuiElement>(seed);
        seedList->insert(seedPtr);
    }
    _selectedSeedInput = _seedInputs.front();
    _trackGenerator.setSeed("AAAAA");

    std::vector<std::unique_ptr<GuiElement>> guiElements;
    guiElements.emplace_back(seedList);

    GuiTextInput* length = new GuiTextInput(6, GuiCharacterInput::CharacterFlags::DIGITS);
    length->setText("040000");
    _lengthInput = length;
    _trackGenerator.setLength(40000);
    length->setOnChange([this, length](const std::string& str)
    {
        std::stringstream sstream(str);
        size_t c;
        sstream >> c;
        size_t remainder = c % 500;
        c -= remainder;
        if(remainder != 0 || c < 3000)
        {
            if(c < 3000)
            {
                c = 3000;
            }
            std::string text = std::to_string(c);
            if(text.size() < str.size())
            {
                text.insert(0, std::string(str.size() - text.size(), '0'));
            }
            length->setText(text);
        }
        LOG("Length: ", c);
        _trackGenerator.setLength(c);
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
    curviness->setValue(3.f);
    _trackGenerator.setCurviness(3);

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

        size_t length = 3000 + rand() % 50000;
        length -= length % 500;
        std::string lengthText = std::to_string(length);
        if(lengthText.size() < 6)
        {
            lengthText.insert(0, 6 - lengthText.size(), '0');
        }
        _lengthInput->setText(lengthText);
        _trackGenerator.setLength(length);

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
    _guiContainer.toggleSelection();

    _forwardRenderer.initialize(&_context.window, 0.f, 0.f, 1.f, 1.f);
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
    _trackGenerator.update(dtSeconds);
    return true;
}

bool PreGameApplicationState::bHandleEvent(const sf::Event& event)
{
    _guiContainer.handleEvent(event);
    if(_input.checkActive())
    {
        _input.update();
        for(const sf::Event& e : _input.getEvents())
        {
            _guiContainer.handleEvent(e);
        }
    }
    return true;
}
