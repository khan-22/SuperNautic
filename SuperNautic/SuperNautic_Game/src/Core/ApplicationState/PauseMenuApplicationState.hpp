#pragma once

#ifndef PAUSE_MENU_APPLICATION_STATE_HPP
#define PAUSE_MENU_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Io/Input.hpp"

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
