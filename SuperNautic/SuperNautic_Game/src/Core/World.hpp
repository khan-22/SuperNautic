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
#include "../GFX/ForwardRenderer.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Camera.h"

class ApplicationContext;

class World
{
public:
    World(ApplicationContext& context);

	void handleEvent(const sf::Event& e);
	void update(float dt);
	void render();

	bool bHasWon();

private:
	Track					_track;
	SegmentHandler			_segmentHandler;
	std::vector<Player>		_players;
	std::vector<PointLight>	_pointLights;
	bool					_bHasWon;

	const ApplicationContext& _context;

	// Indices of current segment for players
	std::vector<unsigned>	_playerSegmentIndices;

	GFX::DeferredRenderer _renderer;

	Camera _camera;
};

#endif //WORLD_HPP
