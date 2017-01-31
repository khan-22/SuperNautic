#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "PauseMenuApplicationState.hpp"
#include "MainMenuApplicationState.hpp"
#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"
#include "AssetCache.hpp"
#include "GuiButton.hpp"
#include "../GFX/SfmlRenderer.hpp"
#include "Camera.h"
#include "PlayApplicationState.hpp"


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
        auto mainMenu = std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context));
        _stack.push(mainMenu);
    }));

    quit->move(0.f, resume->getBoundingRect().height * 1.5f);
    _guiContainer.insert(resume);
    _guiContainer.insert(quit);

    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
}

bool PauseMenuApplicationState::bRender()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_guiContainer);
    renderer.display(_context.window, Camera(90, 5, 5));
    return true;
}

bool PauseMenuApplicationState::bUpdate(float dtSeconds)
{
    return false;
}

bool PauseMenuApplicationState::bHandleEvent(const sf::Event& event)
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
    return false;
}
