#pragma once

#ifndef WORLD_HPP
#define WORLD_HPP

#include "SFML/Graphics/RenderWindow.hpp"
namespace sf
{
    class Event;
}

#include <list>

#include "Core/World/Player.hpp"
#include "Core/Track/Track.hpp"
#include "Core/Utility/Camera.h"
#include "GFX/Rendering/DeferredRenderer.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "Core/Utility/DebugCamera.hpp"

class ApplicationContext;

class World
{
public:
    World(ApplicationContext& context);

	void handleEvent(const sf::Event& e);
	void update(float dt, sf::Window& window);
	void render();

	bool bHasWon();

private:
	Track						_track;
	SegmentHandler				_segmentHandler;
	std::vector<Player>			_players;
	std::vector<PointLight>		_pointLights;
	bool						_bHasWon;

	const ApplicationContext&	_context;

	// Indices of current segment for players
	std::vector<unsigned>		_playerSegmentIndices;

	GFX::DeferredRenderer		_renderer;

	Camera						_camera;
	DebugCamera					_debugCamera;
	bool						_bDebugging;
};

#endif //WORLD_HPP
