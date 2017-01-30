#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

#include "PlayApplicationState.hpp"
#include "MainMenuApplicationState.hpp"
#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"


PlayApplicationState::PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
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
            _stack.pop();
            auto mainMenu = std::unique_ptr<ApplicationState>(new MainMenuApplicationState(_stack, _context));
            _stack.push(mainMenu);
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
