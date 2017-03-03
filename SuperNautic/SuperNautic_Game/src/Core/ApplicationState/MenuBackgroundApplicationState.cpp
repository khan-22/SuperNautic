#include "Core/ApplicationState/MenuBackgroundApplicationState.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"


MenuBackgroundApplicationState::MenuBackgroundApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
{
    _colorFill.setSize(sf::Vector2f(_context.window.getSize().x, _context.window.getSize().y));
    _colorFill.setPosition(0.f, 0.f);
    _colorFill.setFillColor(sf::Color(27 * 0.5f, 173 * 0.5f, 222 * 0.5f, 255));

    sf::Vector2u windowSize = _context.window.getSize();

    float yPos = 0;
    while(yPos < windowSize.y)
    {
        float xPos = rand() % windowSize.x;
        yPos += rand() % 10;

        Bubble bubble;
        bubble.visual.setRadius(10.f);
        bubble.visual.setPosition(xPos, yPos);
        bubble.centerX = xPos;
        bubble.sinePosition = rand() % 5000;
        bubble.visual.setFillColor(sf::Color(255, 255, 255, 10));
        bubble.visual.setOutlineColor(sf::Color(255, 255, 255, 50));
        bubble.visual.setOutlineThickness(1.f);
        _bubbles.push_back(bubble);
    }

}

void MenuBackgroundApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_colorFill);
    for(const Bubble& bubble : _bubbles)
    {
        renderer.render(bubble.visual);
    }
    renderer.display(_context.window);
}

bool MenuBackgroundApplicationState::bUpdate(float dtSeconds)
{
    auto end = std::remove_if(_bubbles.begin(), _bubbles.end(), [](const Bubble& b)
    {
        return b.visual.getPosition().y + b.visual.getRadius() < 0.f;
    });
    _bubbles.erase(end, _bubbles.end());

    for(Bubble& bubble : _bubbles)
    {
        bubble.sinePosition += dtSeconds * 200.f;
        float dx = std::sin(bubble.sinePosition) * 10.f + bubble.centerX - bubble.visual.getPosition().x;
        bubble.visual.move(dx, -dtSeconds * 200.f);
    }

    auto yMaxIt = std::max_element(_bubbles.begin(), _bubbles.end(), [](const Bubble& lhs, const Bubble& rhs)
    {
        return lhs.visual.getPosition().y < rhs.visual.getPosition().y;
    });

    if(yMaxIt != _bubbles.end())
    {
        for(float y = yMaxIt->visual.getPosition().y + yMaxIt->visual.getRadius() * 2.f; y < _context.window.getSize().y + yMaxIt->visual.getRadius() * 2.f; y += rand() % 10)
        {
            float xPos = rand() % _context.window.getSize().x;
            Bubble bubble;
            bubble.visual.setRadius(10.f);
            bubble.visual.setPosition(xPos, y);
            bubble.centerX = xPos;
            bubble.sinePosition = rand() % 5000;
            bubble.visual.setFillColor(sf::Color(255, 255, 255, 10));
            bubble.visual.setOutlineColor(sf::Color(255, 255, 255, 50));
            bubble.visual.setOutlineThickness(1.f);
            _bubbles.push_back(bubble);
        }
    }


    return true;
}


bool MenuBackgroundApplicationState::bHandleEvent(const sf::Event& event)
{
    return true;
}
