#pragma once

#ifndef VICTORY_APPLICATION_STATE_HPP
#define VICTORY_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "ApplicationState.hpp"
#include "Asset.hpp"
#include "GuiContainer.hpp"
#include "Input.hpp"

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
};


#endif //VICTORY_APPLICATION_STATE_HPP
