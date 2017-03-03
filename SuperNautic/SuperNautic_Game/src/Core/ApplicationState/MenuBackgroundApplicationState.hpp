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
    struct Bubble
    {
        sf::CircleShape visual;
        float sinePosition;
        float centerX;
    };

    sf::RectangleShape _colorFill;
    std::vector<Bubble> _bubbles;

};


#endif //MENU_BACKGROUND_APPLICATION_STATE
