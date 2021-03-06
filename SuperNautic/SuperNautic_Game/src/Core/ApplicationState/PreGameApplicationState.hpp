#pragma once

#ifndef PRE_GAME_APPLICATION_STATE_HPP
#define PRE_GAME_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiTrackGeneratorControls.hpp"
#include "Core/Io/Input.hpp"
#include "Core/Gui/GuiTrackGenerator.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/Utility/TrackPresetManager.hpp"
#include "Core/Gui/GuiToolTip.hpp"
#include "Core/Gui/GuiText.hpp"

class GuiPlayerJoinContainer;

class PreGameApplicationState : public ApplicationState
{
public:
    PreGameApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    Asset<sf::Font> _font;
    GuiToolTip _toolTip;
    TrackPresetManager _presetManager;
    GuiTrackGenerator _trackGenerator;
    GuiContainer _guiContainer;
    GuiPlayerJoinContainer* _players;
    GuiText            _title;

    Input _input;

    GFX::SfmlRenderer _sfmlRenderer;
    GFX::ForwardRenderer _forwardRenderer;

	int _playersActive;
};


#endif //PRE_GAME_APPLICATION_STATE_HPP
