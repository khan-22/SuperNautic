#include "SFML/Graphics.hpp"

#include "Core/ApplicationState/TrackGenerationApplicationState.h"
#include "Core/ApplicationState/ApplicationStateStack.hpp"
#include "Core/Asset/AssetCache.hpp"
#include "ApplicationContext.hpp"
#include "Core/Io/Log.hpp"

TrackGenerationApplicationState::TrackGenerationApplicationState(ApplicationStateStack& stack, ApplicationContext& context, Track * track)
	: ApplicationState(stack, context)
	, _track(track)
	, _font(AssetCache<sf::Font, std::string>::get("res/arial.ttf"))
{
	LOG("Started track generation.");
	_track->startNewTrack();
	_bRenderedOnce = false;
	_bDone = false;
	// Text
	_text.setFont(*_font.get());
	_text.setString("Generating track");
	_text.setCharacterSize(50);
	_text.setFillColor(sf::Color::Green);
	_text.setOrigin(200, 0);
	sf::Vector2u windowSize = _context.window.getSize();
	sf::Vector2f center(windowSize.x / 2.f, windowSize.y / 2.f);
	_text.setPosition(sf::Vector2f(center.x, center.y - 60));
	// Background
	_background.setPosition(0, 0);
	_background.setFillColor(sf::Color::Black);
	_background.setSize(sf::Vector2f(sf::Vector2f(windowSize)));
	// Container
	_container.setFillColor(sf::Color::Transparent);
	_container.setOutlineColor(sf::Color::Red);
	_container.setOutlineThickness(5);
	_container.setSize(sf::Vector2f(510, 60));
	_container.setOrigin(255, 0);
	_container.setPosition(sf::Vector2f(center.x, center.y + 30));
	// Filler
	_filler.setFillColor(sf::Color::Blue);
	_filler.setSize(sf::Vector2f(0, 0));
	_filler.setPosition(center.x - 250, center.y + 35);
}

TrackGenerationApplicationState::~TrackGenerationApplicationState()
{
	//Nothing
}

bool TrackGenerationApplicationState::bHandleEvent(const sf::Event & event)
{
	return false;
}

bool TrackGenerationApplicationState::bUpdate(float dtSeconds)
{
	if (_bRenderedOnce)
	{
		if (_bDone)
		{
			_stack.pop();
		}
		if (_track->bGenerate())
		{
			_bDone = true;
		}
		_filler.setSize(sf::Vector2f(_track->getProgression() * 500, 50));
	}
	return false;
}

bool TrackGenerationApplicationState::bRender()
{
	_renderer.render(_background);
	_renderer.render(_text);
	_renderer.render(_container);
	_renderer.render(_filler);
	_renderer.display(_context.window);
	_bRenderedOnce = true;
	return false;
}
