#include "SFML/Graphics/RenderTarget.hpp"

#include <cassert>

#include "Core/Gui/MenuBackground.hpp"



MenuBackground::MenuBackground(size_t width, size_t height)
: _width(width)
, _height(height)
{
    _colorFillTop.setSize(sf::Vector2f(static_cast<float>(_width), _height / 2.f));
    _colorFillBot.setSize(sf::Vector2f(static_cast<float>(_width), _height / 2.f));

    _colorFillTop.setPosition(0.f, 0.f);
    _colorFillBot.setPosition(0.f, _height / 2.f);

    sf::Color colorTop(11, 68, 87, 255);
    sf::Color colorBot(27, 173, 222, 255);
    _colorFillTop.setFillColor(colorTop);
    _colorFillBot.setFillColor(colorBot);

    generateBubble(static_cast<float>(_height));
}

void MenuBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_colorFillBot, states);
    for(const Bubble& bubble : _bubbles)
    {
        target.draw(bubble.visual, states);
    }
    target.draw(_colorFillTop, states);
}

void MenuBackground::update(float dtSeconds)
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
    for(float y = yMaxIt->visual.getPosition().y + _BUBBLE_RADIUS * 2.f; y < _height + _BUBBLE_RADIUS * 2.f; y += rand() % 10)
    {
        generateBubble(y);
    }
}

void MenuBackground::generateBubble(float y)
{
    float xPos = static_cast<float>(rand() % _width);
    Bubble bubble;
    bubble.visual.setRadius(_BUBBLE_RADIUS);
    bubble.visual.setPosition(xPos, y);
    bubble.centerX = xPos;
    bubble.sinePosition = static_cast<float>(rand() % 5000);
    bubble.oscillationAmplitude = 5.f + rand() % 50;
    bubble.visual.setFillColor(sf::Color(255, 255, 255, 10));
    bubble.visual.setOutlineColor(sf::Color(255, 255, 255, 50));
    bubble.visual.setOutlineThickness(1.f);
    _bubbles.push_back(bubble);
}
