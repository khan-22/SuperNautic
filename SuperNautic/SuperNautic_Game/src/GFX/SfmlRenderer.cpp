
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include "SfmlRenderer.hpp"
using namespace GFX;


void SfmlRenderer::render(const sf::Drawable& draw)
{
    _drawCalls.push_back(&draw);
}

void SfmlRenderer::display(sf::RenderTarget& target)
{
    target.pushGLStates();
    for(const sf::Drawable* drawCall : _drawCalls)
    {
        target.draw(*drawCall);
    }
    target.popGLStates();

    _drawCalls.clear();
}
