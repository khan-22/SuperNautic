#include "SFML/Window/Event.hpp"

#include "Core/World/World.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/World/Player.hpp"

#include <cmath>

World::World(ApplicationContext& context)
	: _context{ context }
	, _debugCamera{ 90.0f, context.window.getSize().x, context.window.getSize().y, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 } }
	, _bHasWon(false)
	, _timer(context.window.getSize().x, context.window.getSize().y, context.numPlayers)
	, _progression(context.window.getSize().x, context.window.getSize().y, context.numPlayers)
	, _track(context.track.get())
	, _playerRTs(context.numPlayers)
	, _playerParticleRenderers(context.numPlayers)
	, _playerWindowRenderers(context.numPlayers)
	, _playerParticles(context.numPlayers)
	, _playerPointLights(context.numPlayers)
	, _bDebugging(false)
{
	for (int i = 0; i < _playerParticles.size(); i++)
	{
		_playerParticles[i].init(500, glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f), 0.2f, 7.f, 50.f);
		_playerParticles[i].setBirthColor(glm::vec3(0.0f, 0.6f, 0.3f));
		_playerParticles[i].start();
	}

	for (int i = 0; i < context.numPlayers; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			_playerPointLights[i].push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
		}
	}

    for(size_t i = 0 ; i < _context.numPlayers; i++)
    {
        _players.emplace_back(i);
        _playerProgression.push_back(TrackProgression{ 0, _track });
        LOG(i);

		// Set ship position and rotation
		_players[i].getShip().rotateAtStart(10.0f, glm::pi<float>() * 0.5f * i);

		// Set ship engine cooldown
		_players[i].getShip().setInactiveTime(2.0f);
    }

	// TEST PLAYER

	if (_players.size() == 0)
	{
		_players.emplace_back(10);
		_playerProgression.push_back(TrackProgression{ 0, _track });
	}

	if (_players.size() == 1)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y, 0, 0);
	}
	else if (_players.size() == 2)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, 0);

		_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_playerParticleRenderers[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_playerWindowRenderers[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_players[1].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);
	}
	else if (_players.size() == 3)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_playerWindowRenderers[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);
		_playerRTs[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_playerWindowRenderers[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[2].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, context.window.getSize().y / 2);
	}
	else if (_players.size() == 4)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_players[0].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[1].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_playerWindowRenderers[1].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, context.window.getSize().y / 2);
		_playerRTs[2].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_playerParticleRenderers[2].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_playerWindowRenderers[2].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_players[2].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, 0);
		_playerRTs[3].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[3].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_playerWindowRenderers[3].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[3].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);
	}
}


void World::handleEvent(const sf::Event& e)
{

}

void World::update(float dt, sf::Window& window)
{
	if (!_bDebugging)
	{
		// Update players
		for (unsigned i = 0; i < _players.size(); ++i)
		{
			// Finds forward vector of ship and updates segment index
			glm::vec3 returnPos;
			glm::vec3 directionDifference;
			unsigned segmentIndex = _playerProgression[i].getCurrentSegment();
			float lengthInSegment = 0.0f;
			glm::vec3 forward = _track->findForward(_players[i].getShip().getPosition(), segmentIndex, returnPos, lengthInSegment, directionDifference);

			// Update progression
			_playerProgression[i].setCurrentSegment(segmentIndex);
			_playerProgression[i].update(lengthInSegment);
			_players[i].setProgression(_playerProgression[i].getProgression());

			// Update ship forward position and respawn position
			_players[i].getShip().setForward(forward);
			_players[i].getShip().setReturnPos(returnPos);
			_players[i].getShip().setWaypointDifference(directionDifference);

			// Find segments adjacent to ship
			std::vector<SegmentInstance*> instances;
			for (long j = static_cast<long>(_playerProgression[i].getCurrentSegment() - 1); j <= static_cast<long>(_playerProgression[i].getCurrentSegment()) + 1; ++j)
			{
				if (j >= 0 && j < _track->getNrOfSegments())
				{
					instances.push_back(_track->getInstance(static_cast<int>(j)));
				}
			}

			if (instances[1] == _track->getInstance(_track->getNrOfSegments() - 1))
			{
				_bHasWon = true;
			}

			// Set relevant segments
			_players[i].getShip().setSegments(instances);

			_players[i].update(dt);
		}

		std::vector<float> positions;

		for (unsigned i = 0; i < _players.size(); ++i)
		{
			int k = 1;
			for (unsigned j = 0; j < _players.size(); ++j)
			{
				if (_playerProgression[i].getProgression() < _playerProgression[j].getProgression())
				{
					k++;
				}
			}
			_players[i].setPosition(k);
			positions.push_back(_playerProgression[i].getProgression());
		}

		_progression.updatePositions(positions);

		// Update debug camera
		_debugCamera.setPos(_players[0].getShip().getMeshPosition() -_players[0].getShip().getCameraForward() * 12.0f + _players[0].getShip().getCameraUp() * 4.0f);

		// Commenting this out fixes mouse movement and debug camera rotation desyncing
		//_debugCamera.setUp(_players[0].getShip().getCameraUp());

		//_debugCamera.setUp(_players[0].getShip().getCameraUp());
		_debugCamera.setViewDir(_players[0].getShip().getCameraForward());
	}
	else
	{
		_debugCamera.update(dt, _context.window);
	}
	if (!_bDebugging && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_bDebugging = true;
		_debugCamera.setPos(_players[0].getCamera()->getPosition());
	}
	if (_bDebugging && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		_bDebugging = false;
	}

	_track->update(dt);

	_timer.updateTime(dt);
	_timer.updateCurrent();

	_progression.updateCurrent();

	for (int i = 0; i < _playerParticles.size(); i++)
	{
		glm::vec3 particlePos = _players[i].getShip().getTransform() * glm::vec4{ 0.0f, 0.0f, -1.9f, 1.0f };
		_playerParticles[i].update(dt, particlePos);
	}

	//previousPos = currentPos;
}

void World::render()
{
	std::vector<PointLight> shipLights;

	for (int i = 0; i < _playerRTs.size(); i++)
	{
		//Do not render the player's own ship if they are in first person, but render all other ships as normal
		for(int j = 0; j < _players.size(); j++)
		{
			if (!(i == j && _players[j]._bIsFirstPerson))
			{
				_playerRTs[i].render(_players[j].getShip());
			}
		}
	}

    //////////////////////
    // Copy-pasted from GuiPlayerJoinContainer::createWindows
    static constexpr size_t MAX_PLAYERS = 4;
    static constexpr unsigned char COLORS[4][MAX_PLAYERS] =
    {
        {255, 0, 0, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {255, 255, 0, 255}
    };
	//////////////////////

    assert(_players.size() <= MAX_PLAYERS);
	for(size_t i = 0; i < _players.size(); i++)
	{
	    Player& player = _players[i];
        const unsigned char* c = COLORS[i];
        glm::vec3 diffuseColor(c[0], c[1], c[2]);
        diffuseColor /= 255.f;

		shipLights.push_back(PointLight(player.getShip().getMeshPosition() - player.getShip().getMeshForward() * 2.0f, diffuseColor, 1.5f));
	}

	for (int i = 0; i < _playerRTs.size(); i++)
	{
		for (int j = 0; j < shipLights.size(); j++)
		{
			_playerRTs[i].pushPointLight(shipLights[j]);
		}
	}

	for (int i = 0; i < _playerRTs.size(); i++)
	{
		_track->render(_playerRTs[i], _playerWindowRenderers[i], _playerProgression[i].getCurrentSegment());
	}


	for (int i = 0; i < _playerRTs.size(); i++)
	{
		updateLightPos(_playerProgression[i], i);

		for (int j = 0; j < _playerPointLights[i].size(); j++)
		{
			_playerRTs[i].pushPointLight(_playerPointLights[i][j]);
		}
	}

	if (!_bDebugging)
	{
		for (int i = 0; i < _players.size(); i++)
		{
			_playerRTs[i].display(*_players[i].getCamera());
			_playerRTs[i].blitDepthOnto(GFX::Framebuffer::DEFAULT);
		}
		
		for (int i = 0; i < _playerWindowRenderers.size(); i++)
		{
			_playerWindowRenderers[i].display(*_players[i].getCamera());
		}

		for (int i = 0; i < _playerParticleRenderers.size(); i++)
		{
			for (int j = 0; j < _players.size(); j++)
			{
				_playerParticleRenderers[i].render(_playerParticles[j]);
				_playerParticleRenderers[i].display(*_players[i].getCamera());
			}
		}

	}
	else
	{
		_playerRTs[0].display(_debugCamera);
		_playerRTs[0].blitDepthOnto(GFX::Framebuffer::DEFAULT);

		for (int i = 0; i < _playerWindowRenderers.size(); i++)
		{
			_playerWindowRenderers[i].display(_debugCamera);
		}

		for (int j = 0; j < _players.size(); j++)
		{
			_playerParticleRenderers[0].render(_playerParticles[j]);
			_playerParticleRenderers[0].display(_debugCamera);
		}

	}


	// Should be done for each player before drawing particles.

	GFX::SfmlRenderer sfml;
	for (int i = 0; i < _players.size(); i++)
	{
		sfml.render(_players[i].getHud());
	}

	sfml.render(_timer);
	sfml.render(_progression);

	sfml.display(_context.window);
}

bool World::bHasWon()
{
	return _bHasWon;
}

void World::setTrack(Track * track)
{
	_track = track;
}

void World::updateLightPos(const TrackProgression& playerProgression, int playerIndex)
{
	std::vector<SegmentInstance*> instancesForLights;
	for (long j = static_cast<long>(playerProgression.getCurrentSegment()); j <= static_cast<long>(playerProgression.getCurrentSegment()) + 5; ++j)
	{
		if (j >= 0 && j < _track->getNrOfSegments())
		{
			instancesForLights.push_back(_track->getInstance(static_cast<int>(j)));
		}
	}

	for (int k = 0; k < instancesForLights.size(); k++)
	{
		_playerPointLights[playerIndex][k].setPosition(instancesForLights[k]->getModelMatrix() * glm::vec4(instancesForLights[k]->getParent()->getWaypoints()[0], 1.f));
	}
}
