#pragma once

#ifndef PRE_GAME_APPLICATION_STATE_HPP
#define PRE_GAME_APPLICATION_STATE_HPP

#include "SFML/Graphics/Font.hpp"

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Gui/GuiTrackGeneratorControls.hpp"
#include "Core/Io/Input.hpp"
#include "Core/Gui/GuiTrackGenerator.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/Utility/TrackPresetManager.hpp"


class GuiPlayerJoinContainer;

class PreGameApplicationState : public ApplicationState
{
public:
    PreGameApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    TrackPresetManager _presetManager;
    GuiTrackGenerator _trackGenerator;
    GuiTrackGeneratorControls _guiContainer;
    GuiPlayerJoinContainer* _players;
    Input _input;
    Asset<sf::Font> _font;

    GFX::SfmlRenderer _sfmlRenderer;
    GFX::ForwardRenderer _forwardRenderer;

	int _playersActive;
};


#endif //PRE_GAME_APPLICATION_STATE_HPP
