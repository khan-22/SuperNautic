#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "VictoryApplicationState.hpp"
#include "MainMenuApplicationState.hpp"
#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"
#include "AssetCache.hpp"
#include "GuiButton.hpp"
#include "../GFX/SfmlRenderer.hpp"
#include "Camera.h"
#include "PlayApplicationState.hpp"


VictoryApplicationState::VictoryApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
{
    std::cout << "Welcome to Victory state." << std::endl;

    _guiContainer.setBackground(sf::Color::Magenta);
    sf::Text text;
    text.setFont(*_font.get());
    text.setFillColor(sf::Color::White);

    text.setString("Play again!");
    auto playAgain = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
		auto playState = std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context));
		_stack.push(playState);
    }));

    text.setString("Go to main menu");
    auto quit = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
        auto mainMenu = std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context));
        _stack.push(mainMenu);
    }));

    quit->move(0.f, playAgain->getBoundingRect().height * 1.5f);
    _guiContainer.insert(playAgain);
    _guiContainer.insert(quit);

    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
}

bool VictoryApplicationState::bRender()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_guiContainer);
    renderer.display(_context.window);
    return true;
}

bool VictoryApplicationState::bUpdate(float dtSeconds)
{
    return false;
}

bool VictoryApplicationState::bHandleEvent(const sf::Event& event)
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
