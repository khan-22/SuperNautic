#include "ApplicationState.hpp"
#include "ApplicationStateStack.hpp"

ApplicationState::ApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: _stack(stack)
, _context(context)
{
}

ApplicationState::~ApplicationState()
{
}
