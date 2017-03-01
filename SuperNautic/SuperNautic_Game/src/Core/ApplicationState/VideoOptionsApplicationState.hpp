#pragma once

#ifndef VIDEO_OPTIONS_APPLICATION_STATE_HPP
#define VIDEO_OPTIONS_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiContainer.hpp"
#include "Core/Io/Input.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"


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
    GuiContainer _guiContainer;
    Input _input;
    Asset<sf::Font> _font;
    GFX::SfmlRenderer _sfmlRenderer;

    uint32_t _windowStyle;
    size_t _resolutionX;
    size_t _resolutionY;

    void setResolution(size_t x, size_t y);
    void setFullscreen(bool bSetFullscreen);
    void recreateWindow();
};


#endif //VIDEO_OPTIONS_APPLICATION_STATE_HPP
