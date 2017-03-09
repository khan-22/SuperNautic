#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Core/ApplicationState/PlayApplicationState.hpp"
#include "Core/ApplicationState/VictoryApplicationState.hpp"
#include "Core/ApplicationState/PauseMenuApplicationState.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"


PlayApplicationState::PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
	: ApplicationState(stack, context)
	, _world(context)
	, _bHasRaceEnded(false)
{
    std::cout << "Welcome to Play state. Press ESC to go back to main menu." << std::endl;
}


void PlayApplicationState::render()
{
    _world.render();
}

bool PlayApplicationState::bUpdate(float dtSeconds)
{
    _world.update(dtSeconds, _context.window);

	if (_world.bHasWon() && !_bHasRaceEnded)
	{
		_stack.push(std::unique_ptr<ApplicationState>(new VictoryApplicationState(_stack, _context)));
		_bHasRaceEnded = true;
	}

    if(_input.checkActive())
    {
        _input.update(dtSeconds);
        for(const sf::Event& e : _input.getEvents())
        {
            if(e.type == sf::Event::KeyPressed)
            {
                switch(e.key.code)
                {
                case sf::Keyboard::Escape:
                    _stack.push(std::unique_ptr<ApplicationState>(new PauseMenuApplicationState(_stack, _context, _world)));
                    return true;
                    break;

                default:
                    break;
                }
            }
        }
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
            _stack.push(std::unique_ptr<ApplicationState>(new PauseMenuApplicationState(_stack, _context, _world)));
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
