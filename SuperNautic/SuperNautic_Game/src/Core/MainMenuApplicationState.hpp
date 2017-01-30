#pragma once

#ifndef MAIN_MENU_APPLICATION_STATE_HPP
#define MAIN_MENU_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "ApplicationState.hpp"
#include "Asset.hpp"
#include "GuiContainer.hpp"

class MainMenuApplicationState : public ApplicationState
{
public:
    MainMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    bool bRender() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    GuiContainer _guiContainer;
    Asset<sf::Font> _font;
};


#endif //MAIN_MENU_APPLICATION_STATE_HPP
