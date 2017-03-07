#pragma once

#ifndef OPTIONS_APPLICATION_STATE_HPP
#define OPTIONS_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Io/Input.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/Utility/VideoOptions.hpp"
#include "Core/Utility/AudioOptions.hpp"
#include "Core/Gui/GuiToolTip.hpp"
#include "Core/Gui/GuiText.hpp"

class GuiTextInput;
class GuiSlider;

class OptionsApplicationState : public ApplicationState
{
public:
    OptionsApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;


private:
    VideoOptions _videoOptions;
    AudioOptions _audioOptions;
    GuiContainer _guiContainer;
    Input _input;
    Asset<sf::Font> _font;
    GFX::SfmlRenderer _sfmlRenderer;
    GuiToolTip  _toolTip;
    GuiText _title;

    void applyOptions();
};


#endif //OPTIONS_APPLICATION_STATE_HPP
