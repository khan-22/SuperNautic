#include "SFML/Window/Event.hpp"

#include "Core/World/World.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"

#include <cmath>

World::World(ApplicationContext& context, Track * track)
	: _context{ context }, _camera{ 90.0f, 1280, 720, glm::vec3{0,0,0}, glm::vec3{0,0,1} }
	, _debugCamera{ 90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 } }
	, _bHasWon(false)
	, _timer(1280, 720)
	, _track (track)
{
	_renderer.initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);

	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));

	// Create one player
	for (int i = 0; i < 4; i++)
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

	_bDebugging = false;
}


void World::handleEvent(const sf::Event& e)
{

}

void World::update(float dt, sf::Window& window)
{
	if (!_bDebugging)
	{
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
		_camera.setPos(_players[0].getShip().getMeshPosition() -_players[0].getShip().getCameraForward() * 12.0f + _players[0].getShip().getCameraUp() * 2.0f);
		_camera.setUp(_players[0].getShip().getCameraUp());
		_camera.setViewDir(_players[0].getShip().getCameraForward());
	}
	else
	{
		_debugCamera.update(dt, window);
	}
	if (!_bDebugging && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_bDebugging = true;
		_debugCamera.setPos(_players[0].getShip().getPosition());
	}
	if (_bDebugging && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		_bDebugging = false;
	}

	_timer.updateTime(dt);
	_timer.updateCurrent();
}

void World::render()
{
	std::vector<PointLight> shipLights;
	for (Player& player : _players)
	{
		_renderer.render(player.getShip());
		shipLights.push_back(PointLight(player.getShip().getMeshPosition(), { 1.f,0.5f,0.f }, 1.f));
	}

	for (int i = 0; i < shipLights.size(); i++)
	{
		_renderer.pushPointLight(shipLights[i]);
	}

	_track->render(_renderer, _playerProgression[0].getCurrentSegment());
	//_renderer.render(_track->);


	for (int i = 0; i < _pointLights.size(); i++)
	{
		_renderer.pushPointLight(_pointLights[i]);
	}

	if (!_bDebugging)
	{
		_renderer.display(_camera);
	}
	else
	{
		_renderer.display(_debugCamera);
	}

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
