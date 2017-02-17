#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PlayApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"


MainMenuApplicationState::MainMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
, _input()
{
    std::cout << "Welcome to MainMenu state." << std::endl;

    _guiContainer.setBackground(sf::Color::Magenta);
    sf::Text text;
    text.setFont(*_font.get());
    text.setFillColor(sf::Color::White);

    text.setString("Play");
    auto button1 = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.pop();
        auto playState = std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context, _playersActive));
        _stack.push(playState);
    }));

    text.setString("Quit");
    auto button2 = std::unique_ptr<GuiElement>(new GuiButton(text, [&]()
    {
        _stack.clear();
    }));

    button2->move(0.f, button1->getBoundingRect().height * 1.5f);
    _guiContainer.insert(button1);
    _guiContainer.insert(button2);

    sf::Vector2u windowSize = _context.window.getSize();
    _guiContainer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
}

bool MainMenuApplicationState::bRender()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_guiContainer);
    renderer.display(_context.window);
//    _context.window.draw(_guiContainer);
    return true;
}

bool MainMenuApplicationState::bUpdate(float dtSeconds)
{
	int playersFound = 0;
	for (int i = 0; i < 5; i++)
	{
		if (sf::Joystick::isConnected(i)) {
			playersFound++;
		}
	}
	_playersActive = playersFound;
	if (_playersActive == 0)
	{
		_playersActive = 1;
	}
    return true;
}

bool MainMenuApplicationState::bHandleEvent(const sf::Event& event)
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
