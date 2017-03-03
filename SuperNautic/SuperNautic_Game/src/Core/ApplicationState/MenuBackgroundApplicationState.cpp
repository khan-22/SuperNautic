#include "Core/ApplicationState/MenuBackgroundApplicationState.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"


MenuBackgroundApplicationState::MenuBackgroundApplicationState(ApplicationStateStack& stack, ApplicationContext& context)
: ApplicationState(stack, context)
{
    _colorFillTop.setSize(sf::Vector2f(_context.window.getSize().x, _context.window.getSize().y / 2.f));
    _colorFillBot.setSize(sf::Vector2f(_context.window.getSize().x, _context.window.getSize().y / 2.f));

    _colorFillTop.setPosition(0.f, 0.f);
    _colorFillBot.setPosition(0.f, _context.window.getSize().y / 2.f);

    sf::Color colorTop(11, 68, 87, 255);
    sf::Color colorBot(27, 173, 222, 255);
    _colorFillTop.setFillColor(colorTop);
    _colorFillBot.setFillColor(colorBot);

    sf::Vector2u windowSize = _context.window.getSize();
    generateBubble(_context.window.getSize().y);
}

void MenuBackgroundApplicationState::render()
{
    GFX::SfmlRenderer renderer;
    renderer.render(_colorFillBot);
    for(const Bubble& bubble : _bubbles)
    {
        renderer.render(bubble.visual);
    }
    renderer.render(_colorFillTop);
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
        static float radiansPerSecond = 3.14f / 4.f;
        static float speedY = 100.f;

        bubble.sinePosition += dtSeconds * radiansPerSecond;
        float dx = std::sin(bubble.sinePosition) * bubble.oscillationAmplitude + bubble.centerX - bubble.visual.getPosition().x;
        bubble.visual.move(dx, -dtSeconds * speedY);
    }

    auto yMaxIt = std::max_element(_bubbles.begin(), _bubbles.end(), [](const Bubble& lhs, const Bubble& rhs)
    {
        return lhs.visual.getPosition().y < rhs.visual.getPosition().y;
    });

    assert(yMaxIt != _bubbles.end());
    for(float y = yMaxIt->visual.getPosition().y + _BUBBLE_RADIUS * 2.f; y < _context.window.getSize().y + _BUBBLE_RADIUS * 2.f; y += rand() % 10)
    {
        generateBubble(y);
    }


    return true;
}


bool MenuBackgroundApplicationState::bHandleEvent(const sf::Event& event)
{
    return true;
}


void MenuBackgroundApplicationState::generateBubble(float y)
{
    float xPos = rand() % _context.window.getSize().x;
    Bubble bubble;
    bubble.visual.setRadius(_BUBBLE_RADIUS);
    bubble.visual.setPosition(xPos, y);
    bubble.centerX = xPos;
    bubble.sinePosition = rand() % 5000;
    bubble.oscillationAmplitude = 5.f + rand() % 50;
    bubble.visual.setFillColor(sf::Color(255, 255, 255, 10));
    bubble.visual.setOutlineColor(sf::Color(255, 255, 255, 50));
    bubble.visual.setOutlineThickness(1.f);
    _bubbles.push_back(bubble);
}
