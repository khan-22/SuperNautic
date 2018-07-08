#pragma once

namespace sf
{
    class Event;
}

namespace GFX
{
    class SfmlRenderer;
}

class TextSystem
{
    public:
        static void handle_event(const sf::Event& event);
        static void update(GFX::SfmlRenderer& renderer);
};