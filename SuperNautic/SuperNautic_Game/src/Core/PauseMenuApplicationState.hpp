#pragma once

#ifndef PAUSE_MENU_APPLICATION_STATE_HPP
#define PAUSE_MENU_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "ApplicationState.hpp"
#include "Asset.hpp"
#include "GuiContainer.hpp"
#include "Input.hpp"

class PauseMenuApplicationState : public ApplicationState
{
public:
    PauseMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    bool bRender() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    GuiContainer _guiContainer;
    Input _input;
    Asset<sf::Font> _font;
};


#endif //PAUSE_MENU_APPLICATION_STATE_HPP
