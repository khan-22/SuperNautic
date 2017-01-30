#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

#include "PlayApplicationState.hpp"
#include "ApplicationStateStack.hpp"
#include "ApplicationContext.hpp"


PlayApplicationState::PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
{
    std::cout << "Welcome to Play state." << std::endl;
}

bool PlayApplicationState::bRender()
{
    return true;
}

bool PlayApplicationState::bUpdate(float dtSeconds)
{
    return true;
}

bool PlayApplicationState::bHandleEvent(const sf::Event& event)
{
    return true;
}
