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
#include "DebugCamera.hpp"

class ApplicationContext;

class World
{
public:
    World(ApplicationContext& context);

	void handleEvent(const sf::Event& e);
	void update(float dt, sf::Window& window);
	void render();

private:
	Track						_track;
	SegmentHandler				_segmentHandler;
	std::vector<Player>			_players;
	std::vector<PointLight>		_pointLights;

	const ApplicationContext&	_context;

	// Indices of current segment for players
	std::vector<unsigned>		_playerSegmentIndices;

	GFX::DeferredRenderer		_renderer;

	Camera						_camera;
	DebugCamera					_debugCamera;
	bool						_bDebugging;
};

#endif //WORLD_HPP
