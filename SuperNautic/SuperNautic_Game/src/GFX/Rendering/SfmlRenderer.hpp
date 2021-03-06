#pragma once

#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

namespace sf
{
    class Drawable;
    class RenderTarget;
}

#include <list>

namespace GFX
{
    class SfmlRenderer
    {
        public:
            void render(const sf::Drawable& draw);
            void display(sf::RenderTarget& target);

        private:
            std::list<const sf::Drawable*> _drawCalls;
    };
}


#endif //SFML_RENDERER_HPP
