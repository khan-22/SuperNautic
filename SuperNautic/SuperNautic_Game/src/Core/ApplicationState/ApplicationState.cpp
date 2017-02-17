#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/ApplicationState/ApplicationStateStack.hpp"

ApplicationState::ApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: _stack(stack)
, _context(context)
{
}

ApplicationState::~ApplicationState()
{
}

void ApplicationState::initialize()
{

}