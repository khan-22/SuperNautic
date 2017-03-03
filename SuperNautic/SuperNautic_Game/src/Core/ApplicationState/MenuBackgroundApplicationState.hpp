#pragma once

#ifndef MENU_BACKGROUND_APPLICATION_STATE
#define MENU_BACKGROUND_APPLICATION_STATE

#include <memory>
#include <vector>

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"

class MenuBackgroundApplicationState : public ApplicationState
{
public:
    MenuBackgroundApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    static constexpr float _BUBBLE_RADIUS = 10.f;
    struct Bubble
    {
        sf::CircleShape visual;
        float sinePosition;
        float oscillationAmplitude;
        float centerX;
    };

    sf::RectangleShape _colorFillTop;
    sf::RectangleShape _colorFillBot;
    std::vector<Bubble> _bubbles;


    void generateBubble(float y);

};


#endif //MENU_BACKGROUND_APPLICATION_STATE
