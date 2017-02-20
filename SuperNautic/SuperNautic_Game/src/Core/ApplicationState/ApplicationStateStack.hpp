#pragma once

#ifndef APPLICATION_STATE_STACK_HPP
#define APPLICATION_STATE_STACK_HPP

#include <list>
#include <queue>
#include <memory>

namespace sf
{
    class Event;
}

#include "Core/ApplicationState/ApplicationState.hpp"

class ApplicationStateStack
{
public:
    // Ctor...
    ApplicationStateStack();

    // Update all states with a timestep.
    void update(float dtSeconds);

    // Render all states.
    void render();

    // Handle input event for all states.
    void handleEvent(const sf::Event& event);

    // Push a state onto the top of the stack.
    void push(std::unique_ptr<ApplicationState>&& state);

    // Pop the state on the top of the stack.
    void pop();

    // Pop all states from the stack.
    void clear();

    // Return true if stack contains no states.
    bool bIsEmpty() const;

private:
    enum Request
    {
        Push,
        Pop,
        Clear,
    };

    std::list<std::unique_ptr<ApplicationState>>    _stack;
    std::queue<std::unique_ptr<ApplicationState>>   _pushQueue;
    std::queue<Request>                             _requestQueue;

    void applyPendingRequests();
};

#endif //APPLICATION_STATE_STACK_HPP
