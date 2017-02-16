#include "SFML/Window/Event.hpp"

#include "Core/World/World.hpp"
#include "Core/ApplicationState/ApplicationContext.hpp"
#include "Core/Track/SegmentInstance.hpp"
#include "GFX/Rendering/SfmlRenderer.hpp"

#include <cmath>

World::World(ApplicationContext& context)
	: _segmentHandler{ "Segments/segmentinfos.txt", "Segments/ConnectionTypes.txt" }, _track{ &_segmentHandler }
	, _context{ context }, _camera{ 90.0f, 1280, 720, glm::vec3{0,0,0}, glm::vec3{0,0,1} }
	, _debugCamera{ 90.0f, 1280, 720, glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 } }
	, _bHasWon(false)
	, _timer(1280, 720)
{
	_renderer.initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);

	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 6.f));

	// Create one player
	for (int i = 0; i < 5; i++)
	{
		if (sf::Joystick::isConnected(i)) {
			_players.emplace_back(i);
			_playerSegmentIndices.push_back(0);
			LOG(i);
		}
	}

	// TEST PLAYER
	if (_players.size() == 0)
	{
		_players.emplace_back(10);
		_playerSegmentIndices.push_back(0);
	}
	//_players.emplace_back(0);
	//_players.emplace_back(2);
	//_players.emplace_back(3);
	//_players.emplace_back(4);

	//_playerSegmentIndices.push_back(0);
	//_playerSegmentIndices.push_back(0);
	//_playerSegmentIndices.push_back(0);
	//_playerSegmentIndices.push_back(0);

	_track.setLength(10000);
	_track.setSeed(1);
	_track.generate();
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
			glm::vec3 forward = _track.findForward(_players[i].getShip().getPosition(), _playerSegmentIndices[i], returnPos);
			_players[i].getShip().setForward(forward);
			_players[i].getShip().setReturnPos(returnPos);

			// Find segments adjacent to ship
			std::vector<SegmentInstance*> instances;
			for (long j = static_cast<long>(_playerSegmentIndices[i] - 1); j <= static_cast<long>(_playerSegmentIndices[i]) + 1; ++j)
			{
				if (j >= 0 && j < _track.getNrOfSegments())
				{
					instances.push_back(_track.getInstance(static_cast<int>(j)));
				}

			}

			if (instances[1] == _track.getInstance(_track.getNrOfSegments() - 1))
			{
				_bHasWon = true;
			}

			std::vector<SegmentInstance*> instancesForLights;
			for (long j = static_cast<long>(_playerSegmentIndices[i]); j <= static_cast<long>(_playerSegmentIndices[i]) + 5; ++j)
			{
				if (j >= 0 && j < _track.getNrOfSegments())
				{
					instancesForLights.push_back(_track.getInstance(static_cast<int>(j)));
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
		///////////TEST CAMERA////////////
		//_camera.setPos(_players[0].getShip().getMeshPosition() - _players[0].getShip().getCameraForward() * 12.0f + _players[0].getShip().getCameraUp() * 2.0f);
		//_camera.setUp(_players[0].getShip().getCameraUp());
		//_camera.setViewDir(_players[0].getShip().getCameraForward());
		/////////////////////////////////

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
		shipLights.push_back(PointLight(player.getShip().getPosition(), { 1.f,0.5f,0.f }, 1.f));
	}

	for (int i = 0; i < shipLights.size(); i++)
	{
		_renderer.pushPointLight(shipLights[i]);
	}

	_track.render(_renderer, _playerSegmentIndices[0]);
	//_renderer.render(_track.);


	for (int i = 0; i < _pointLights.size(); i++)
	{
		_renderer.pushPointLight(_pointLights[i]);
	}

	if (!_bDebugging)
	{
		_renderer.display(*_players[0].getCamera());
		//_renderer.display(_camera);
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
