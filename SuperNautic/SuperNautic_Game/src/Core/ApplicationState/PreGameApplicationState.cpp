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

PreGameApplicationState::PreGameApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
{
    // curviness slider
    // seed text ALL + horizontal list
    // length text DIGITS

    GuiHorizontalList* seedList = new GuiHorizontalList();
    seedList->setOnElementSelect([&](GuiElement* selection)
    {
        LOG("Set new seed: \"", ((GuiTextInput*)selection)->getText(), "\"");
    });

    for(size_t i = 0; i < 10; i++)
    {
        GuiTextInput* seed = new GuiTextInput(5, GuiCharacterInput::CharacterFlags::ALL);
        seed->setOnChange([](const std::string& str)
        {
            LOG("Seed: \"", str, "\"");
        });
        seed->setText(std::string(5, 'A' + i));
        auto seedPtr = std::unique_ptr<GuiElement>(seed);
        seedList->insert(seedPtr);
    }


    std::vector<std::unique_ptr<GuiElement>> guiElements;
    guiElements.emplace_back(seedList);

    GuiTextInput* length = new GuiTextInput(6, GuiCharacterInput::CharacterFlags::DIGITS);
    length->setOnChange([length](const std::string& str)
    {
        std::stringstream sstream(str);
        size_t c;
        sstream >> c;
        size_t remainder = c % 500;
        if(remainder != 0)
        {
            std::string text = std::to_string(c - remainder);
            if(text.size() < str.size())
            {
                text.insert(0, std::string(str.size() - text.size(), '0'));
            }
            length->setText(text);
        }
        LOG("Length: ", c);
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
    curviness->setOnChange([&](float c)
    {
        LOG("Curviness: ", (int)c);
        _trackGenerator.setCurviness(c);
    });
//    curviness->setOrigin(curviness->getBoundingRect().width / 2.f, curviness->getBoundingRect().height / 2.f);
    guiElements.emplace_back(curviness);

    sf::Vector2f pos(0.f, 0.f);
    for(const std::unique_ptr<GuiElement>& e : guiElements)
    {
        e->setPosition(pos);
        pos.y += e->getBoundingRect().height * 2.f;
    }

    _guiContainer.setBackground(sf::Color(255, 255, 255, 50));
//    sf::Text text;
//    text.setFont(*_font.get());
//    text.setFillColor(sf::Color::White);
//
//    text.setString("Play");
//    auto button1 = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
//    {
//        _stack.pop();
//        auto playState = std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context, 1));
//        _stack.push(std::move(playState));
//
//    }));
//
//    text.setString("Quit");
//    auto button2 = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
//    {
//        _stack.clear();
//    }));
//
//    button2->move(0.f, button1->getBoundingRect().height * 1.5f);
//    _guiContainer.insert(button1);
//    _guiContainer.insert(button2);

    _guiContainer.insert(guiElements);

    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    _guiContainer.toggleSelection();

    _forwardRenderer.initialize(&_context.window, 0.f, 0.f, 1.f, 1.f);

    _trackGenerator.setSeed(0);
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
