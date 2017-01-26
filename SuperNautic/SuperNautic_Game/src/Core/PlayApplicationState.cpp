#include "PlayApplicationState.hpp"
#include "ApplicationStateStack.hpp"

PlayApplicationState::PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
{

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
