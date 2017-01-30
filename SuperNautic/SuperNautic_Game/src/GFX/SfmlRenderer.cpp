
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include "SfmlRenderer.hpp"

namespace GFX
{

    template<>
    void SfmlRenderer::render(const sf::Drawable& draw)
    {
        _drawCalls.push_back(&draw);
    }

    template<>
    void SfmlRenderer::display(sf::RenderTarget& target, const Camera& camera)
    {
        for(const sf::Drawable* drawCall : _drawCalls)
        {
            target.draw(*drawCall);
        }
    }
}
