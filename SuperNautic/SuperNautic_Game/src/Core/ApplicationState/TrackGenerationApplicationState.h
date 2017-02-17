#pragma once
#ifndef TRACKGENERATIONAPPLICATIONSTATE_H
#define TRACKGENERATIONAPPLICATIONSTATE_H

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Track/Track.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "SFML/Graphics/Text.hpp"

class TrackGenerationApplicationState : public ApplicationState
{
public:
	TrackGenerationApplicationState(ApplicationStateStack& stack, ApplicationContext& context, Track * track);
	virtual ~TrackGenerationApplicationState();
	bool bHandleEvent(const sf::Event& event) override;
	bool bUpdate(float dtSeconds) override;
	bool bRender() override;

private:
	Track *				_track;
	GFX::SfmlRenderer	_renderer;
	Asset<sf::Font>		_font;
	sf::Text			_text;
	sf::RectangleShape	_background;
	sf::RectangleShape	_container;
	sf::RectangleShape	_filler;
	bool				_bRenderedOnce;
	bool				_bDone;
};

#endif // !TRACKGENERATIONAPPLICATIONSTATE_H
