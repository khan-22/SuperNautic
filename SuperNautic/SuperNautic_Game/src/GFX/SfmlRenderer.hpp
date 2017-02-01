#pragma once

#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

namespace sf
{
    class Drawable;
}

namespace GFX
{
    class SfmlRenderer
    {
        public:
            void render(const sf::Drawable& draw);
            void display(sf::RenderTarget& target, const Camera& camera);
    };
}


#endif //SFML_RENDERER_HPP
