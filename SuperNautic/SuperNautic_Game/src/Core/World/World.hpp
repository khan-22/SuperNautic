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
#include "Core/Audio/CountdownAudio.hpp"
#include "Core/Utility/Camera.h"
#include "Core/Utility/DebugCamera.hpp"
#include "Core/Track/TrackProgression.hpp"
#include "Core/Gui/Timer.hpp"
#include "Core/Gui/Progression.hpp"
#include "GFX/Rendering/DeferredRenderer.hpp"
#include "GFX/Rendering/ZoneRenderer.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "GFX/Rendering/WindowRenderer.hpp"
#include "GFX/Rendering/TransparentRenderer.hpp"
#include "GFX/Rendering/ParticleRenderer.hpp"

#include "GFX/Resources/ParticleSystem.hpp"

#include "GFX/Rendering/ViewportPipeline.hpp"

struct ApplicationContext;

class World
{
public:
	World(ApplicationContext& context);
	~World();

	void handleEvent(const sf::Event& e);
	void update(float dt, sf::Window& window);
	void render();

	bool bHasWon();
	void setTrack(Track * track);

	void pause();
	void resume();

private:
	Track*									_track;
	std::vector<Player>						_players;
	std::vector<std::vector<PointLight>>	_playerPointLights;

	unsigned int								_playersAtFinishLine;

	Timer									_timer;
	Progression								_progression;

	CountdownAudio							_countdown;

	const ApplicationContext&				_context;

	// Keeps track of indices and progress of players
	std::vector<TrackProgression>	_playerProgression;

	std::vector<GFX::ViewportPipeline>	_viewportPipelines;

	/*std::vector<GFX::DeferredRenderer>	_playerRTs;
	std::vector<GFX::ZoneRenderer>		_playerZoneRenderers;
	std::vector<GFX::ParticleRenderer>	_playerParticleRenderers;
	std::vector<GFX::WindowRenderer>	_playerWindowRenderers;*/

	uint64_t						_frameCount;
	float							_playTime;

	DebugCamera						_debugCamera;
	bool							_bDebugging;

	void updateLightPos(const TrackProgression& playerProgression, int playerIndex);
};

#endif //WORLD_HPP
