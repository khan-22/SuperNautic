#include "Core/ApplicationState/ApplicationStateStack.hpp"


ApplicationStateStack::ApplicationStateStack()
{
}

void ApplicationStateStack::update(float dtSeconds)
{
    applyPendingRequests();

    for(auto it = _stack.rbegin(); it != _stack.rend(); it++)
    {
        if(!(*it)->bUpdate(dtSeconds))
        {
            break;
        }
    }

    applyPendingRequests();
}

void ApplicationStateStack::render()
{
    applyPendingRequests();

    std::list<ApplicationState*> visibleStates;
    for(auto it = _stack.rbegin(); it != _stack.rend(); it++)
    {
        visibleStates.push_front((*it).get());
        if(!(*it)->bIsTransparent())
        {
            break;
        }
    }

    applyPendingRequests();

    for(ApplicationState* visibleState : visibleStates)
    {
        visibleState->render();
    }

    applyPendingRequests();
}

void ApplicationStateStack::push(std::unique_ptr<ApplicationState>& state)
{
    _pushQueue.push(std::move(state));
    _requestQueue.push(Push);
}

void ApplicationStateStack::pop()
{
    _requestQueue.push(Pop);
}

void ApplicationStateStack::clear()
{
    _requestQueue.push(Clear);
}

bool ApplicationStateStack::bIsEmpty() const
{
    return _stack.empty();
}

void ApplicationStateStack::handleEvent(const sf::Event& event)
{
    applyPendingRequests();

    for(auto it = _stack.rbegin(); it != _stack.rend(); it++)
    {
        if(!(*it)->bHandleEvent(event))
        {
            break;
        }
    }

    applyPendingRequests();
}

void ApplicationStateStack::applyPendingRequests()
{
    while(!_requestQueue.empty())
    {
        switch(_requestQueue.front())
        {
            case Push:
                _stack.push_back(std::move(_pushQueue.front()));
				_stack.back()->initialize();
                _pushQueue.pop();
                break;

            case Pop:
                _stack.pop_back();
                break;

            case Clear:
                _stack.clear();
                break;
        }

        _requestQueue.pop();
    }
}
