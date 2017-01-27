#pragma once

#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

namespace sf
{
    class RenderWindow;
}

struct ApplicationContext
{
    ApplicationContext(sf::RenderWindow& window)
    : window(window)
    {
    }

    sf::RenderWindow& window;
};


#endif //APPLICATION_CONTEXT_HPP
