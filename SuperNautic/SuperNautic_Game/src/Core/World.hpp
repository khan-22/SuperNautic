#pragma once

#ifndef WORLD_HPP
#define WORLD_HPP

namespace sf
{
    class Event;
}

#include <list>

#include "Player.hpp"
#include "Track/Track.hpp"
#include "../GFX/DeferredRenderer.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "DebugCamera.hpp"

class ApplicationContext;

class World
{
public:
    World(ApplicationContext& context);

	void handleEvent(const sf::Event& e);
	void update(float dt);
	void render();

private:
	Track				_track;
	SegmentHandler		_segmentHandler;
	std::vector<Player>	_players;

	const ApplicationContext& _context;

	// Indices of current segment for players
	std::vector<unsigned>	_playerSegmentIndices;

	GFX::DeferredRenderer _renderer;

	DebugCamera _debugCamera;
};

#endif //WORLD_HPP
