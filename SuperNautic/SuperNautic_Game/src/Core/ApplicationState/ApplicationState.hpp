#pragma once

#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

namespace sf
{
    class Event;
}

class ApplicationStateStack;
struct ApplicationContext;

class ApplicationState
{
	public:
	    // Ctor...
        ApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

        // Dtor...
        virtual ~ApplicationState();

        // Render the state
        virtual void render() = 0;

        // Update the state with a timestep
        // Return false to prevent updating states further down the stack.
        virtual bool bUpdate(float dtSeconds) = 0;

        // Handle an input event
        // Return false to prevent event handling for states further down the stack.
        virtual bool bHandleEvent(const sf::Event& event) = 0;

        // Return false to prevent rendering of states further down the stack.
        virtual bool bIsTransparent();

		virtual void initialize();

    protected:
        ApplicationStateStack&  _stack;
        ApplicationContext&     _context;
};


#endif //APPLICATION_STATE_HPP
