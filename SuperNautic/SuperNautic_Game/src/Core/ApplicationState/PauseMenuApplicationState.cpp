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
#include "Core/World/World.hpp"

PauseMenuApplicationState::PauseMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context, World& world)
: ApplicationState(stack, context)
, _world(world)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
, _title("Paused", _font)
{
    std::cout << "Welcome to PauseMenu state." << std::endl;

    _guiContainer.setBackground(sf::Color::Magenta);
    sf::Text text;
    text.setFont(*_font.get());
    text.setFillColor(sf::Color::White);


    text.setString("Quit race");
    auto quit = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
        _stack.push(std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context)));
    }));

    _guiContainer.insert(quit);

    _guiContainer.setBackground(sf::Color(27, 173, 222, 100), sf::Color(19, 121, 156, 100), 5.f);


    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    _guiContainer.toggleSelection();

    _title.setCharacterSize(50);
    _title.setOrigin(_title.getBoundingRect().width / 2.f, _title.getBoundingRect().height / 2.f);
    _title.setPosition(windowSize.x / 2.f, _guiContainer.getBoundingRect().top / 2.f);

    _background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    _background.setPosition(0.f, 0.f);
    _background.setFillColor(sf::Color(0, 0, 0, 180));

    _world.pause();
}

void PauseMenuApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_background);
    renderer.render(_guiContainer);
    renderer.render(_title);
    renderer.display(_context.window);
}

bool PauseMenuApplicationState::bUpdate(float dtSeconds)
{
    if(_input.checkActive())
    {
        _input.update();
        for(const sf::Event& e : _input.getEvents())
        {
            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::Escape:
                case sf::Keyboard::B:
                    _stack.pop();
                    return false;

                default:
                    break;
                }
            }
            _guiContainer.handleEvent(e);
        }
    }
    return false;
}

bool PauseMenuApplicationState::bHandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Escape:
        case sf::Keyboard::B:
            _stack.pop();
            _world.resume();
            return false;

        default:
            break;
        }
    }
    _guiContainer.handleEvent(event);
    return false;
}
