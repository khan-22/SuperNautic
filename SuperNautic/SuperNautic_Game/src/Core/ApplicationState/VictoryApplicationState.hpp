#pragma once

#ifndef VICTORY_APPLICATION_STATE_HPP
#define VICTORY_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Io/Input.hpp"

class VictoryApplicationState : public ApplicationState
{
public:
	VictoryApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    bool bRender() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    GuiContainer _guiContainer;
    Input _input;
    Asset<sf::Font> _font;

	int _playersActive;
};


#endif //VICTORY_APPLICATION_STATE_HPP
