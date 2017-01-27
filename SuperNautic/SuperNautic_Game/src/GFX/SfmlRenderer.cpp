
#include "SFML/Graphics/Drawable.hpp"


#include "Renderer.hpp"

namespace GFX
{
    template<>
    void Renderer<sf::Drawable, sf::Transform>::render(const sf::Drawable& draw, const sf::Transform& transform)
    {

    }
}
