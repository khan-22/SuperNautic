#pragma once

#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

#include "Renderer.hpp"

#include "SFML/Graphics/Drawable.hpp"

namespace GFX
{
    using SfmlRenderer = Renderer<sf::Drawable, sf::Transform>;
}


#endif //SFML_RENDERER_HPP
