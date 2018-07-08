#pragma once

namespace sf
{
    class Event;
}

class MenuItemSystem
{
    public:
        static void handle_event(const sf::Event& event);
        static void update();
};
