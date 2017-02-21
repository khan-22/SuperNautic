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
#include "Core/Utility/DebugCamera.hpp"
#include "Core/Track/TrackProgression.hpp"
#include "Core/Gui/Timer.hpp"
#include "GFX/Rendering/DeferredRenderer.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"

#include "GFX/Rendering/ParticleRenderer.hpp"
#include "GFX/Resources/ParticleSystem.hpp"

struct ApplicationContext;

class World
{
public:
    World(ApplicationContext& context, Track * track, const int numberOfPlayers);

	void handleEvent(const sf::Event& e);
	void update(float dt, sf::Window& window);
	void render();

	bool bHasWon();
	void setTrack(Track * track);

private:
	Track*							_track;
	std::vector<Player>				_players;
	std::vector<PointLight>			_pointLights;
	bool							_bHasWon;

	Timer							_timer;

	const ApplicationContext&		_context;

	// Keeps track of indices and progress of players
	std::vector<TrackProgression>	_playerProgression;

	//GFX::DeferredRenderer		_renderer;

	std::vector<GFX::DeferredRenderer> _playerRTs;

	Camera						_camera;
	bool						_bDebugging;

	GFX::ParticleSystem		_testParticles;
	GFX::ParticleRenderer	_particleRenderer;
};

#endif //WORLD_HPP
