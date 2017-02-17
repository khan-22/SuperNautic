#include "SFML/Window/Event.hpp"

#include "Core/World/World.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"

#include <cmath>

World::World(ApplicationContext& context, Track* track, const int numberOfPlayers)
	: _context{ context }
	, _debugCamera{ 90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 } }
	, _bHasWon(false)
	, _timer(1280, 720)
	, _track(track)
	, _playerRTs(numberOfPlayers)
{

	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));

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

	if (_players.size() == 1)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);
		_players[0].setScreenSize(1280, 720, 0, 0);
	}
	else if (_players.size() == 2)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(1280, 360, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 1.0f, 0.5f);
		_players[1].setScreenSize(1280, 360, 0, 360);
	}
	else if (_players.size() == 3)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 1.0f, 0.5f);
		_players[0].setScreenSize(1280, 360, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[1].setScreenSize(640, 360, 0, 360);
		_playerRTs[2].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
		_players[2].setScreenSize(640, 360, 640, 360);
	}
	else if (_players.size() == 4)
	{
		_playerRTs[0].initialize(&context.window, 0.0f, 0.5f, 0.5f, 0.5f);
		_players[0].setScreenSize(640, 360, 0, 0);
		_playerRTs[1].initialize(&context.window, 0.5f, 0.5f, 0.5f, 0.5f);
		_players[1].setScreenSize(640, 360, 640, 0);
		_playerRTs[2].initialize(&context.window, 0.0f, 0.0f, 0.5f, 0.5f);
		_players[2].setScreenSize(640, 360, 0, 360);
		_playerRTs[3].initialize(&context.window, 0.5f, 0.0f, 0.5f, 0.5f);
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

	for (int i = 0; i < _playerRTs.size(); i++)
	{
		for (Player& player : _players)
		{
			_playerRTs[i].render(player.getShip());
		}
	}
	for (Player& player : _players)
	{
		shipLights.push_back(PointLight(player.getShip().getPosition(), { 1.f,0.5f,0.f }, 1.f)); //TODO Don't remake lights each tick, retard
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
		}
	}
	else
	{
		_playerRTs[0].display(_debugCamera);
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
