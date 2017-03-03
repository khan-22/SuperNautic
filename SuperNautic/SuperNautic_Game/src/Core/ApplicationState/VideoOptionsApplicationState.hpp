#pragma once

#ifndef VIDEO_OPTIONS_APPLICATION_STATE_HPP
#define VIDEO_OPTIONS_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Io/Input.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/Utility/VideoOptions.hpp"
#include "Core/Gui/GuiToolTip.hpp"

class GuiTextInput;
class GuiSlider;

class VideoOptionsApplicationState : public ApplicationState
{
public:
    VideoOptionsApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;


private:
    VideoOptions _videoOptions;
    GuiContainer _guiContainer;
    Input _input;
    Asset<sf::Font> _font;
    GFX::SfmlRenderer _sfmlRenderer;
    GuiToolTip  _toolTip;

    void applyOptions();
};


#endif //VIDEO_OPTIONS_APPLICATION_STATE_HPP
