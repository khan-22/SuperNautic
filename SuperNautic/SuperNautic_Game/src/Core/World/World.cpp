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
	, _playersAtFinishLine(0)
	, _timer(context.window.getSize().x, context.window.getSize().y, static_cast<int>(context.players.size()))
	, _progression(context.window.getSize().x, context.window.getSize().y, static_cast<int>(context.players.size()))
	, _track(context.track.get())
	, _playerRTs(context.players.size())
	, _playerParticleRenderers(context.players.size())
	, _playerWindowRenderers(context.players.size())
	, _playerZoneRenderers(context.players.size())
	, _playerPointLights(context.players.size())
	, _viewportPipelines(context.players.size())
	, _bDebugging(false)
{
	for (int i = 0; i < context.players.size(); i++)
	{
		for (int j = 0; j < 6; j++)
		{
			_playerPointLights[i].push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
		}
	}

	std::vector<glm::vec3> colors{ glm::vec3{ 0.8f, 0.2f, 0.1f },
		glm::vec3{ 0.1f, 0.8f, 0.1f },
		glm::vec3{ 0.1f, 0.1f, 0.8f },
		glm::vec3{ 0.8f, 0.8f, 0.1f } };

	_players.reserve(context.players.size());
    for(int i = 0 ; i < context.players.size(); i++)
    {
        GuiPlayerJoinContainer::Player player = context.players[i];
		_players.emplace_back(player.id, colors[i]);
        _playerProgression.push_back(TrackProgression{ 0, _track });
        LOG(i);

		// Set ship position and rotation
		_players[i].getShip().rotateAtStart(7.0f, glm::pi<float>() * 0.5f * i);
    }

	//Create a render target for each player
	if (_players.size() == 0)
	{
		/*_players.emplace_back(10);
		_playerProgression.push_back(TrackProgression{ 0, _track });*/

		LOG_ERROR("No players detected!");
		abort();
	}

	if (_players.size() == 1)
	{
		//_playerRTs[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		//_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		//_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		//_playerZoneRenderers[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y, 0, 0);
	}
	else if (_players.size() == 2)
	{
		//_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		//_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		//_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		//_playerZoneRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);

		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, 0);

		//_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		//_playerParticleRenderers[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		//_playerWindowRenderers[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		//_playerZoneRenderers[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);

		_viewportPipelines[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_players[1].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);
	}
	else if (_players.size() == 3)
	{
		//_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		//_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		//_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		//_playerZoneRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);

		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, 0);

		//_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		//_playerParticleRenderers[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		//_playerWindowRenderers[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		//_playerZoneRenderers[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);

		_viewportPipelines[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);

		//_playerRTs[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		//_playerParticleRenderers[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		//_playerWindowRenderers[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		//_playerZoneRenderers[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		
		_viewportPipelines[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[2].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, context.window.getSize().y / 2);
	}
	else if (_players.size() == 4)
	{
		//_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		//_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		//_playerWindowRenderers[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		//_playerZoneRenderers[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_players[0].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, 0);

		//_playerRTs[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		//_playerParticleRenderers[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		//_playerWindowRenderers[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		//_playerZoneRenderers[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);

		_viewportPipelines[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, 0);

		//_playerRTs[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		//_playerParticleRenderers[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		//_playerWindowRenderers[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		//_playerZoneRenderers[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_viewportPipelines[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[2].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);

		//_playerRTs[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		//_playerParticleRenderers[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		//_playerWindowRenderers[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		//_playerZoneRenderers[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);

		_viewportPipelines[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[3].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, context.window.getSize().y / 2);
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
				if (!_players[i].getShip().getStopped())
				{
					++_playersAtFinishLine;
				}
				_players[i].getShip().stop();
			}

			// Set relevant segments
			if (!_players[i].getShip().getStopped())
			{
				_players[i].getShip().setSegments(instances);
			}
			
			_players[i].update(dt);

			// Check for ship-ship collisions
			for (unsigned j = i + 1; j < _players.size(); ++j)
			{
				if (bTestCollision(_players[i].getShip().getBoundingBox(), _players[j].getShip().getBoundingBox()))
				{
					if (!bAlmostEqual(_players[i].getShip().getBoundingBox().center - _players[j].getShip().getBoundingBox().center, glm::vec3{ 0.0f }))
					{
						_players[i].getShip().setBounce(glm::normalize(_players[i].getShip().getBoundingBox().center - _players[j].getShip().getBoundingBox().center) * 2.0f);
						_players[j].getShip().setBounce(glm::normalize(_players[j].getShip().getBoundingBox().center - _players[i].getShip().getBoundingBox().center) * 2.0f);
						// Play sound
						_players[i].shipCollision();
					}
				}
			}
		}

		std::vector<float> progression;

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
			progression.push_back(_playerProgression[i].getProgression());
		}

		_progression.updatePositions(progression);

		// Update debug camera
		_debugCamera.setPos(_players[0].getShip().getMeshPosition() - _players[0].getShip().getCameraForward() * 12.0f + _players[0].getShip().getCameraUp() * 4.0f);

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
	// Get segment index of first and last player to send to track.update(...)
	size_t max = 0, min = _track->getNrOfSegments() - 1;
	for (size_t i = 0; i < _playerProgression.size(); i++)
	{
		size_t segment = _playerProgression[i].getCurrentSegment();
		if (segment > max)
		{
			max = segment;
		}
		if (segment < min)
		{
			min = segment;
		}
	}
	_track->update(dt, static_cast<unsigned>(max), static_cast<unsigned>(min));

	_timer.updateTime(dt);
	_timer.updateCurrent();

	_progression.updateCurrent();
}

void World::render()
{
	for (int i = 0; i < _playerRTs.size(); i++)
	{
		//Do not render the player's own ship if they are in first person, but render all other ships as normal
		for (int j = 0; j < _players.size(); j++)
		{
			if (!(i == j && _players[j]._bIsFirstPerson))
			{
				_viewportPipelines[i].generalDeferred.render(_players[j].getShip());
			}
		}
	}
	for (int i = 0; i < _playerRTs.size(); i++)
	{
		for (int j = 0; j < _players.size(); j++)
		{
			_viewportPipelines[i].generalDeferred.pushPointLight(_players[j].getShip().getPointLight());

			_viewportPipelines[i].generalDeferred.pushPointLight(_players[j].getShip().getWarningLight());
		}
	}

	for (int i = 0; i < _playerRTs.size(); i++)
	{
		_track->render(_viewportPipelines[i], _playerProgression[i].getCurrentSegment());
	}

	for (int i = 0; i < _playerParticleRenderers.size(); i++)
	{
		for (int j = 0; j < _players.size(); j++)
		{
			_viewportPipelines[i].particleForward.render(_players[j].getShip().getParticleSystem());
		}
	}

	if (!_bDebugging)
	{
		/*for (int i = 0; i < _players.size(); i++)
		{
			_playerRTs[i].display(*_players[i].getCamera());
			_playerRTs[i].blitDepthOnto(GFX::Framebuffer::DEFAULT);
		}

		for (int i = 0; i < _playerZoneRenderers.size(); i++)
		{
			_playerZoneRenderers[i].display(*_players[i].getCamera());
		}

		for (int i = 0; i < _playerWindowRenderers.size(); i++)
		{
			_playerWindowRenderers[i].display(*_players[i].getCamera());
		}

		for (int i = 0; i < _playerParticleRenderers.size(); i++)
		{
			_playerParticleRenderers[i].display(*_players[i].getCamera());
		}
*/

		for (int i = 0; i < _players.size(); i++)
		{
			_viewportPipelines[i].display(*_players[i].getCamera());
		}
	}
	else
	{
		/*_playerRTs[0].display(_debugCamera);
		_playerRTs[0].blitDepthOnto(GFX::Framebuffer::DEFAULT);

		_playerZoneRenderers[0].display(_debugCamera);
		
		_playerWindowRenderers[0].display(_debugCamera);
		
		_playerParticleRenderers[0].display(_debugCamera);*/

		_viewportPipelines[0].display(_debugCamera);
	}

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
	return _playersAtFinishLine >= _players.size();
}

void World::setTrack(Track * track)
{
	_track = track;
}
