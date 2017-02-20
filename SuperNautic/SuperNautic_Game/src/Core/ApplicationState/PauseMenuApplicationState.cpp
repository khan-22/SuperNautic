#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/PauseMenuApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PlayApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"


PauseMenuApplicationState::PauseMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
{
    std::cout << "Welcome to PauseMenu state." << std::endl;

    _guiContainer.setBackground(sf::Color::Magenta);
    sf::Text text;
    text.setFont(*_font.get());
    text.setFillColor(sf::Color::White);

    text.setString("Resume");
    auto resume = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.pop();
    }));

    text.setString("Quit race");
    auto quit = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
    }));

    quit->move(0.f, resume->getBoundingRect().height * 1.5f);
    _guiContainer.insert(resume);
    _guiContainer.insert(quit);

    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
}

void PauseMenuApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_guiContainer);
    renderer.display(_context.window);
}

bool PauseMenuApplicationState::bUpdate(float dtSeconds)
{
    return false;
}

bool PauseMenuApplicationState::bHandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        if(event.key.code == sf::Keyboard::Escape)
        {
            _stack.pop();
            return false;
        }
    }
    _guiContainer.handleEvent(event);
    if(_input.checkActive())
    {
        _input.update();
        for(const sf::Event& e : _input.getEvents())
        {
            _guiContainer.handleEvent(e);
        }
    }
    return false;
}
