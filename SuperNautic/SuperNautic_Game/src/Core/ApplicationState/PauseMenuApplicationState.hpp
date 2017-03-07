#pragma once

#ifndef PAUSE_MENU_APPLICATION_STATE_HPP
#define PAUSE_MENU_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Io/Input.hpp"
#include "Core/Gui/GuiText.hpp"
class World;


class PauseMenuApplicationState : public ApplicationState
{
public:
    PauseMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context, World& world);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    World& _world;
    sf::RectangleShape _background;
    GuiContainer _guiContainer;
    Input _input;
    Asset<sf::Font> _font;
    GuiText _title;
};


#endif //PAUSE_MENU_APPLICATION_STATE_HPP
