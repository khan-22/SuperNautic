#include "SFML/Window/Event.hpp"

#include "Core/World/World.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"
#include "Core/World/Player.hpp"

#include <cmath>

World::World(ApplicationContext& context, Track* track, const int numberOfPlayers)
	: _context{ context }
	, _camera{ 90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 } }
	, _bHasWon(false)
	, _timer(1280, 720)
	, _track(track)
	, _playerRTs(numberOfPlayers)
	, _playerParticleRenderers(numberOfPlayers)
	, _playerParticles(numberOfPlayers)
{
	for (int i = 0; i < _playerParticles.size(); i++)
	{
		_playerParticles[i].init(500, glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f), 0.2f, 7.f, 50.f);
	}

	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));

	// Create one player
	for (int i = 0; i < 5; i++)
	{
		if (sf::Joystick::isConnected(i)) {
			_players.emplace_back(i);
			_playerProgression.push_back(TrackProgression{ 0, _track });
			LOG(i);
		}
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
		_players[0].setScreenSize(1280, 720, 0, 0);
	}
	else if (_players.size() == 2)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(1280, 360, 0, 0);

		_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_playerParticleRenderers[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_players[1].setScreenSize(1280, 360, 0, 360);
	}
	else if (_players.size() == 3)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(1280, 360, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[1].setScreenSize(640, 360, 0, 360);
		_playerRTs[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[2].setScreenSize(640, 360, 640, 360);
	}
	else if (_players.size() == 4)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_playerParticleRenderers[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_players[0].setScreenSize(640, 360, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[1].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[1].setScreenSize(640, 360, 640, 0);
		_playerRTs[2].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_playerParticleRenderers[2].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_players[2].setScreenSize(640, 360, 0, 360);
		_playerRTs[3].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_playerParticleRenderers[3].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[3].setScreenSize(640, 360, 640, 360);
	}

	_bDebugging = false;
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
			unsigned segmentIndex = _playerProgression[i].getCurrentSegment();
			float lengthInSegment = 0.0f;
			glm::vec3 forward = _track->findForward(_players[i].getShip().getPosition(), segmentIndex, returnPos, lengthInSegment);
			
			// Update progression
			_playerProgression[i].setCurrentSegment(segmentIndex);
			_playerProgression[i].update(lengthInSegment);
			_players[i].setProgression(_playerProgression[i].getProgression());

			// LOG("PROGRESSION: ", _playerProgression[i].getProgression());

			// Update ship forward position and respawn position
			_players[i].getShip().setForward(forward);
			_players[i].getShip().setReturnPos(returnPos);

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

			std::vector<SegmentInstance*> instancesForLights;
			for (long j = static_cast<long>(_playerProgression[i].getCurrentSegment()); j <= static_cast<long>(_playerProgression[i].getCurrentSegment()) + 5; ++j)
			{
				if (j >= 0 && j < _track->getNrOfSegments())
				{
					instancesForLights.push_back(_track->getInstance(static_cast<int>(j)));
				}
			}

			for (int k = 0; k < instancesForLights.size(); k++)
			{
				_pointLights[k].setPosition(instancesForLights[k]->getModelMatrix() * glm::vec4(instancesForLights[k]->getParent()->getWaypoints()[0], 1.f));
			}

			// Set relevant segments
			_players[i].getShip().setSegments(instances);

			_players[i].update(dt);
		}

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
		}

		// Update debug camera
		_camera.setPos(_players[0].getShip().getMeshPosition() -_players[0].getShip().getCameraForward() * 12.0f + _players[0].getShip().getCameraUp() * 4.0f);
		_camera.setUp(_players[0].getShip().getCameraUp());
		_camera.setViewDir(_players[0].getShip().getCameraForward());
	}
	else
	{
	}
	if (!_bDebugging && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_bDebugging = true;
	}
	if (_bDebugging && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		_bDebugging = false;
	}

	_timer.updateTime(dt);
	_timer.updateCurrent();

	//static glm::vec3 currentPos = _players[0].getShip().getPosition();
	//static glm::vec3 previousPos = currentPos;
	//currentPos = glm::vec3{ _players[0].getShip().getMatrix() * glm::vec4{ 0.f,0.f,0.f,1.f } } - _players[0].getShip().getMeshForward() * 2.0f;//_players[0].getShip().getPosition();
	
	//_testParticles.update(dt, currentPos, currentPos - previousPos);
	for (int i = 0; i < _playerParticles.size(); i++)
	{
		glm::vec3 particlePos = _players[i].getShip().getMeshPosition() - _players[i].getShip().getMeshForward() * 2.0f;
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
	for (Player& player : _players)
	{
		//shipLights.push_back(PointLight(player.getShip().getMeshPosition() - player.getShip().getMeshForward() * 3.0f, { 1.f,0.5f,0.f }, 1.f)); //TODO Don't remake lights each tick, retard
		shipLights.push_back(PointLight(player.getShip().getMeshPosition() - player.getShip().getMeshForward() * 2.0f, { 1.f,0.5f,0.f }, 1.f));

		// TEST
		//shipLights.push_back(PointLight(glm::vec3{ player.getShip().getMatrix() * glm::vec4{ -2,0,0,1 } }, { 0.f,0.5f,1.f }, 1.f));
		//shipLights.push_back(PointLight(glm::vec3{ player.getShip().getMatrix() * glm::vec4{ 0,0,0,1 } } +player.getShip().getMeshForward() * 2.0f, { 1.f,0.5f,1.f }, 1.f));
		//shipLights.push_back(PointLight(glm::vec3{ player.getShip().getMatrix() * glm::vec4{0,0,0,1} } - player.getShip().getMeshForward() * 2.0f, { 1.f,0.0f,0.f }, 1.f));
		///////
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
		_track->render(_playerRTs[i], _playerProgression[i].getCurrentSegment());
	}


	for (int i = 0; i < _playerRTs.size(); i++)
	{
		for (int j = 0; j < _pointLights.size(); j++)
		{
			_playerRTs[i].pushPointLight(_pointLights[j]);
		}
	}

	if (!_bDebugging)
	{
		for (int i = 0; i < _players.size(); i++)
		{
			_playerRTs[i].display(*_players[i].getCamera());
			_playerRTs[i].blitDepthOnto(GFX::Framebuffer::DEFAULT);
		}
	}
	else
	{
		_playerRTs[0].display(_camera);
		_playerRTs[0].blitDepthOnto(GFX::Framebuffer::DEFAULT);
	}


	for (int i = 0; i < _playerParticleRenderers.size(); i++)
	{
		for (int j = 0; j < _players.size(); j++)
		{
			_playerParticleRenderers[i].render(_playerParticles[j]);
			_playerParticleRenderers[i].display(*_players[i].getCamera());
		}
	}

	// Should be done for each player before drawing particles.

	//_particleRenderer.render(_testParticles);
	//_particleRenderer.display(*_players[0].getCamera());

	GFX::SfmlRenderer sfml;
	for (Player& player : _players)
	{
		sfml.render(player.getHud());
	}

	sfml.render(_timer);

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
