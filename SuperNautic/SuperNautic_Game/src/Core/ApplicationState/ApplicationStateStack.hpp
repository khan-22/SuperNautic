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
    ApplicationStateStack();

    void update(float dtSeconds);
    void render();
    void handleEvent(const sf::Event& event);

    void push(std::unique_ptr<ApplicationState>& state);
    void pop();
    void clear();
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
