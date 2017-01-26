#pragma once

#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

namespace sf
{
    class Event;
}

class ApplicationStateStack;
class ApplicationContext;

class ApplicationState
{
	public:
        ApplicationState(ApplicationStateStack& stack, ApplicationContext& context);
        virtual ~ApplicationState();

        virtual bool bRender() = 0; // Return false to prevent rendering of states in front of this one.
        virtual bool bUpdate(float dtSeconds) = 0; // Return false to prevent updating of states behind this one.
        virtual bool bHandleEvent(const sf::Event& event) = 0; // Return false to prevent event handling of states behind this one.

    protected:
        ApplicationStateStack& _stack;
        ApplicationContext& _context;
};


#endif //APPLICATION_STATE_HPP
