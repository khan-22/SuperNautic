#pragma once

#ifndef ABOUT_APPLICATION_STATE_HPP
#define ABOUT_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Gui/GuiText.hpp"
#include "Core/Io/Input.hpp"


class AboutApplicationState : public ApplicationState
{
public:
    AboutApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    SceneNode _guiGraph;
    FontAsset _font;
    Input _input;
};


#endif //ABOUT_APPLICATION_STATE_HPP
