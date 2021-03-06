#pragma once

#ifndef MAIN_MENU_APPLICATION_STATE_HPP
#define MAIN_MENU_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Gui/GuiText.hpp"
#include "Core/Io/Input.hpp"


class MainMenuApplicationState : public ApplicationState
{
public:
    MainMenuApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    GuiContainer _guiContainer;
    FontAsset _font;
    GuiText _titleText;
    Input _input;

	int _playersActive;
};


#endif //MAIN_MENU_APPLICATION_STATE_HPP
