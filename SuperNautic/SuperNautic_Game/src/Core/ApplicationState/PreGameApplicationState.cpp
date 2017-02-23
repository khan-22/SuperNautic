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
    GuiHorizontalList* seedList = new GuiHorizontalList();
    seedList->setOnElementSelect([&](GuiElement* selection)
    {
        std::string text = ((GuiTextInput*)selection)->getText();
        LOG("Set new seed: \"", text, "\"");
        _trackGenerator.setSeed(text);
        _trackGenerator.generate();
    });

    for(size_t i = 0; i < 10; i++)
    {
        GuiTextInput* seed = new GuiTextInput(5, GuiCharacterInput::CharacterFlags::ALL);
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
    _trackGenerator.setSeed("AAAAA");

    std::vector<std::unique_ptr<GuiElement>> guiElements;
    guiElements.emplace_back(seedList);

    GuiTextInput* length = new GuiTextInput(6, GuiCharacterInput::CharacterFlags::DIGITS);
    length->setText("040000");
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
    curviness->setValue(3.f);
    _trackGenerator.setCurviness(3);

    curviness->setOnChange([&](float c)
    {
        LOG("Curviness: ", (int)c);
        _trackGenerator.setCurviness(c);
        _trackGenerator.generate();
    });
    guiElements.emplace_back(curviness);

    sf::Vector2f pos(0.f, 0.f);
    for(const std::unique_ptr<GuiElement>& e : guiElements)
    {
        e->setPosition(pos);
        pos.y += e->getBoundingRect().height * 2.f;
    }

    _guiContainer.setBackground(sf::Color(255, 255, 255, 50));
    _guiContainer.insert(guiElements);

    sf::Vector2u windowSize = _context.window.getSize();
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
