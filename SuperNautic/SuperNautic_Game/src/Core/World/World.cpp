#include "SFML/Window/Event.hpp"

#include "Core/World/World.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/World/Player.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"
#include "Core/Geometry/CollisionMesh.hpp"

#include "Core/Utility/Timer.hpp"

#include "gal/gal.hpp"

#include <cmath>



World::World(ApplicationContext& context)
	: _context{ context }
	, _debugCamera{ 90.0f, context.window.getSize().x, context.window.getSize().y, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 } }
	, _playersAtFinishLine(0)
	, _timer(context.window.getSize().x, context.window.getSize().y, static_cast<int>(context.players.size()))
	, _progression(context.window.getSize().x, context.window.getSize().y, static_cast<int>(context.players.size()))
	, _track(context.track.get())
	//, _playerRTs(context.players.size())
	//, _playerParticleRenderers(context.players.size())
	//, _playerWindowRenderers(context.players.size())
	//, _playerZoneRenderers(context.players.size())
	, _playerPointLights(context.players.size())
	, _viewportPipelines(context.players.size())
	, _bDebugging(false)
	, _frameCount(0)
	, _playTime(0.f)
{
	for (int i = 0; i < context.players.size(); i++)
	{
		for (int j = 0; j < 6; j++)
		{
			_playerPointLights[i].push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
		}
	}

	_track->setNrOfPlayers(context.players.size());

	std::vector<glm::vec3> colors{ glm::vec3{ 0.4f, 0.1f, 0.8f },
		glm::vec3{ 0.1f, 0.8f, 0.1f },
		glm::vec3{ 0.1f, 0.1f, 0.8f },
		glm::vec3{ 0.8f, 0.8f, 0.1f } };

	_players.reserve(context.players.size());
    for(int i = 0 ; i < context.players.size(); i++)
    {
        GuiPlayerJoinContainer::Player player = context.players[i];
		_players.emplace_back(player.inputId, colors[i], player.shipId);
		//_players.emplace_back(player.id, colors[i], player.maxAcceleration, player.maxTurningSpeed, player.cooldownOnObstacleCollision, player.overheatTemperature, player.overheatCooldown);
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

	GLuint resultColorChannels[] = { 4 };
	_resultFramebuffer.initialize(_context.window.getSize().x, _context.window.getSize().y, resultColorChannels, sizeof(resultColorChannels) / sizeof(resultColorChannels[0]));

	GFX::Framebuffer* targetFramebuffer = &GFX::Framebuffer::DEFAULT;

	//_darkZonePass.initialize(0.f, 0.f, 1.f, 1.f, &_resultFramebuffer, "darkpass_forward");

	// Randomize seaweed positions
	RawMeshAsset loadedBox{ RawMeshCache::get("seaweed_boundingbox.kmf") };
	BoundingBox seaweedBox{ loadedBox.get()->meshes[0] };

	constexpr float seaweedHeight = 10000.0f;
	for (unsigned i = 0; i < _track->getNrOfSegments(); i += 1)
	{
		glm::vec3 segmentPos = _track->getInstance(i)->getModelMatrix() * glm::vec4{ 0, 0, 0, 1 };

		glm::vec3 position{ segmentPos.x + ((static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f) * 300.0f,
			-seaweedHeight,
			segmentPos.z + ((static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f) * 300.0f };

		seaweedBox.center = position + glm::vec3{ 0.0f, seaweedHeight, 0.0f };
		CollisionMesh seaweedBoundingBox{ seaweedBox };

		if (!_track->bTestCollision(seaweedBoundingBox))
			_seaweeds.emplace_back(position);
	}

	if (_players.size() == 1)
	{
		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f, targetFramebuffer);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y, 0, 0);
	}
	else if (_players.size() == 2)
	{
		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f, targetFramebuffer);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, 0);

		_viewportPipelines[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f, targetFramebuffer);
		_players[1].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);

		//_viewportPipelines[0].initialize(&context.window, 0.0f, 0.0f, 0.5f, 1.0f);
		//_players[0].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y, 0, 0);

		//_viewportPipelines[1].initialize(&context.window, 0.5f, 0.0f, 0.5f, 1.0f);
		//_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y, context.window.getSize().x / 2, 0);
	}
	else if (_players.size() == 3)
	{
		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f, targetFramebuffer);
		_players[0].setScreenSize(context.window.getSize().x, context.window.getSize().y / 2, 0, 0);

		_viewportPipelines[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f, targetFramebuffer);
		_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);

		_viewportPipelines[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f, targetFramebuffer);
		_players[2].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, context.window.getSize().y / 2);
	}
	else if (_players.size() == 4)
	{
		_viewportPipelines[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f, targetFramebuffer);
		_players[0].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, 0);

		_viewportPipelines[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f, targetFramebuffer);
		_players[1].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, 0);

		_viewportPipelines[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f, targetFramebuffer);
		_players[2].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, 0, context.window.getSize().y / 2);

		_viewportPipelines[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f, targetFramebuffer);
		_players[3].setScreenSize(context.window.getSize().x / 2, context.window.getSize().y / 2, context.window.getSize().x / 2, context.window.getSize().y / 2);
	}


	_countdown.playCountdown();
}

World::~World()
{
	LOG("---------------------------");
	LOG("Total Framecount that round was: ", _frameCount);
	LOG("Total Playtime (s) that round was: ", _playTime);
	LOG("Calculated average FPS: ", _frameCount / _playTime);
	LOG("---------------------------");

}


void World::handleEvent(const sf::Event& e)
{

}

void World::pause()
{
    _countdown.pause();
    for(Player& p : _players)
    {
        p.pause();
    }
}

void World::resume()
{
    _countdown.resume();
    for(Player& p : _players)
    {
        p.resume();
    }
}

void World::update(float dt, sf::Window& window)
{
	prof::Timer timer("World::update");
	if (!_countdown.isPlaying())
	{
		_timer.updateTime(dt);
		for(Player& p : _players)
        {
            p.getShip().setInactiveTime(0.f);
        }
	}
	else
    {
		for(Player& p : _players)
        {
            p.getShip().setInactiveTime(_countdown.getTimeLeft());
        }
    }

	if (!_bDebugging)
	{
		// Update players
		for (unsigned int i = 0; i < _players.size(); ++i)
		{
			prof::Timer timer("World::updatePlayer");
			
			// Finds forward vector of ship and updates segment index
			glm::vec3 returnPos;
			glm::vec3 directionDifference;
			unsigned int segmentIndex = _playerProgression[i].getCurrentSegment();
			float lengthInSegment = 0.0f;
			glm::vec3 forward = _track->findForward(_players[i].getShip().getPosition(), segmentIndex, returnPos, lengthInSegment, directionDifference);

			// Update progression
			{
				prof::Timer timer("World::updatePlayerProgression");
				_playerProgression[i].setCurrentSegment(segmentIndex);
				_playerProgression[i].update(lengthInSegment);
			}

			// Update ship forward position and respawn position
			{
				prof::Timer timer("World::updatePlayerShipForward");
				_players[i].getShip().setForward(forward);
				_players[i].getShip().setReturnPos(returnPos);
				_players[i].getShip().setWaypointDifference(directionDifference);
			}

			// Find segments adjacent to ship
			std::vector<SegmentInstance*> instances;
			{

				prof::Timer timer("World::updatePlayerAdjacentSegments");
				for (long j = static_cast<long>(_playerProgression[i].getCurrentSegment() - 1); j <= static_cast<long>(_playerProgression[i].getCurrentSegment()) + 1; ++j)
				{
					if (j >= 0 && j < _track->getNrOfSegments())
					{
						instances.push_back(_track->getInstance(static_cast<int>(j)));
					}
				}

				if (((instances[1] == _track->getInstance(_track->getNrOfSegments() - 1)) && (lengthInSegment > 330.0f)))// || instances[1] == _track->getInstance(_track->getNrOfSegments() - 1))
				{
					if (!_players[i].getShip().getStopped())
					{
						++_playersAtFinishLine;
						_players[i].setPosition(_playersAtFinishLine);
						_players[i].setTime(_timer.getTime());
					}
					_players[i].getShip().stop();
				}

				// Set relevant segments
				if (!_players[i].getShip().getStopped())
				{
					_players[i].getShip().setSegments(instances);
				}
			}

			_players[i].update(dt);

			// Check for ship-ship collisions
			{

				prof::Timer timer("World::updatePlayerCollisions");
				for (unsigned int j = i + 1; j < _players.size(); ++j)
				{
					if (bTestCollision(_players[i].getShip().getBoundingBox(), _players[j].getShip().getBoundingBox()))
					{
						if (!bAlmostEqual(_players[i].getShip().getBoundingBox().center - _players[j].getShip().getBoundingBox().center, glm::vec3{ 0.0f }))
						{
							_players[i].getShip().setBounce(glm::normalize(_players[i].getShip().getBoundingBox().center - _players[j].getShip().getBoundingBox().center) * 0.7f);
							_players[j].getShip().setBounce(glm::normalize(_players[j].getShip().getBoundingBox().center - _players[i].getShip().getBoundingBox().center) * 0.7f);
							// Play sound
							_players[i].shipCollision();
						}
					}
				}
			}
		}

		std::vector<float> progression;

		for (unsigned int i = 0; i < _players.size(); ++i)
		{
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
	//Update track
	std::vector<unsigned int> playerIndexes;
	playerIndexes.reserve(_playerProgression.size());
	for (unsigned int i = 0; i < _playerProgression.size(); i++)
	{
		playerIndexes.push_back(_playerProgression[i].getCurrentSegment());
	}
	_track->update(dt, playerIndexes);


	_timer.updateCurrent(dt);

	_progression.updateCurrent(dt);

	_playTime += dt;
}

void World::render()
{
	prof::Timer timer("World::render");

	gal::commit();


	for (int i = 0; i < _viewportPipelines.size(); i++)
	{
		//Do not render the player's own ship if they are in first person, but render all other ships as normal
		for (int j = 0; j < _players.size(); j++)
		{
			if (!(i == j && _players[j]._bIsFirstPerson))
			{
				_viewportPipelines[i].generalForward.render(_players[j].getShip());
			}
		}
	}
	for (int i = 0; i < _viewportPipelines.size(); i++)
	{
		for (int j = 0; j < _players.size(); j++)
		{
			_viewportPipelines[i].generalForward.pushPointLight(_players[j].getShip().getPointLight());

			_viewportPipelines[i].generalForward.pushPointLight(_players[j].getShip().getWarningLight());
		}
	}

	for (int i = 0; i < _viewportPipelines.size(); i++)
	{
		for (size_t j = 0; j < _seaweeds.size(); ++j)
		{
			glm::vec3 difference{ _players[i].getShip().getPosition().x - _seaweeds[j].getPosition().x, 0.0f, _players[i].getShip().getPosition().z - _seaweeds[j].getPosition().z };

			if (glm::length(difference) < 1000.0f)
			{
				_viewportPipelines[i].windowForward.render(_seaweeds[j]);
			}
		}
		_track->render(_viewportPipelines[i], i, _playerProgression[i].getCurrentSegment());
	}

	for (int i = 0; i < _viewportPipelines.size(); i++)
	{
		for (int j = 0; j < _players.size(); j++)
		{
			_viewportPipelines[i].particleForward.render(_players[j].getShip().getParticleSystem());
			_viewportPipelines[i].particleForward.render(_players[j].getShip().getLeftParticleSystem());
			_viewportPipelines[i].particleForward.render(_players[j].getShip().getRightParticleSystem());
		}
	}
	_resultFramebuffer.bindBoth();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GFX::Framebuffer::DEFAULT.bindBoth();

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

		//_darkZonePass.perform();
	}
	else
	{
		/*_playerRTs[0].display(_debugCamera);
		_playerRTs[0].blitDepthOnto(GFX::Framebuffer::DEFAULT);

		_playerZoneRenderers[0].display(_debugCamera);

		_playerWindowRenderers[0].display(_debugCamera);

		_playerParticleRenderers[0].display(_debugCamera);*/

		_viewportPipelines[0].display(_debugCamera);
		//_darkZonePass.perform();
	}

	GFX::SfmlRenderer sfml;
	for (int i = 0; i < _players.size(); i++)
	{
		sfml.render(_players[i].getHud());
	}

	sfml.render(_timer);
	sfml.render(_progression);

	sfml.display(_context.window);


	_frameCount++;
}

bool World::bHasWon()
{
	return _playersAtFinishLine >= _players.size();
}

void World::setTrack(Track * track)
{
	_track = track;
}
