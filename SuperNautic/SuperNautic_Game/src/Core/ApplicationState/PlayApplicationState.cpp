#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

#include "PlayApplicationState.hpp"
#include "VictoryApplicationState.hpp"
#include "PauseMenuApplicationState.hpp"
#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"


PlayApplicationState::PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
, _world(context)
{
    std::cout << "Welcome to Play state. Press ESC to go back to main menu." << std::endl;
}

bool PlayApplicationState::bRender()
{
    _world.render();
    return true;
}

bool PlayApplicationState::bUpdate(float dtSeconds)
{
    _world.update(dtSeconds);

	if (_world.bHasWon())
	{
		auto victoryState = std::unique_ptr<ApplicationState>(new VictoryApplicationState(_stack, _context));
		_stack.push(victoryState);
	}

    return true;
}

bool PlayApplicationState::bHandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Escape:
        {
            auto pauseMenu = std::unique_ptr<ApplicationState>(new PauseMenuApplicationState(_stack, _context));
            _stack.push(pauseMenu);
            return true;
            break;

        }

        default:
            break;
        }
    }
    _world.handleEvent(event);
    return true;
}
