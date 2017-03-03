#pragma once

#ifndef MENU_BACKGROUND_HPP
#define MENU_BACKGROUND_HPP

#include <vector>

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

class MenuBackground : public sf::Drawable
{
public:
    MenuBackground(size_t width, size_t height);

    void update(float dtSeconds);

private:
    static constexpr float _BUBBLE_RADIUS = 10.f;
    struct Bubble
    {
        sf::CircleShape visual;
        float sinePosition;
        float oscillationAmplitude;
        float centerX;
    };

    size_t _width;
    size_t _height;
    sf::RectangleShape _colorFillTop;
    sf::RectangleShape _colorFillBot;
    std::vector<Bubble> _bubbles;


    void generateBubble(float y);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif //MENU_BACKGROUND_HPP
