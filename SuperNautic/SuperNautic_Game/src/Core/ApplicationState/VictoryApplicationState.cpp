#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/ApplicationState/VictoryApplicationState.hpp"
#include "Core/ApplicationState/MainMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/PlayApplicationState.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "Core/Gui/GuiButton.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/SfmlRenderer.hpp"


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
		auto playState = std::unique_ptr<ApplicationState>(new PlayApplicationState(_stack, _context, _playersActive));
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
	int playersFound = 0;
	for (int i = 0; i < 5; i++)
	{
		if (sf::Joystick::isConnected(i)) {
			playersFound++;
		}
	}
	_playersActive = playersFound;
	if (_playersActive < 1)
	{
		_playersActive = 1;
	}
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
