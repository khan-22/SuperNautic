#include "SFML/Window/Event.hpp"

#include "World.hpp"
#include "ApplicationContext.hpp"
#include "../GFX/SfmlRenderer.hpp"
#include "Track/SegmentInstance.hpp"


World::World(ApplicationContext& context)
	: _segmentHandler{ "Segments/segmentinfos1.txt", "Segments/ConnectionTypes.txt" }, _track{ &_segmentHandler }, _context{ context }, _camera{ 90.0f, 1280, 720, glm::vec3{0,0,0}, glm::vec3{0,0,1} }
	, _bHasWon(false)
{
	_renderer.initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);

	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));
	_pointLights.push_back(PointLight({ 0.f, 0.f, 0.f }, { 0.3f, 0.8f, 1.0f }, 3.f));

	// Create one player
	_players.emplace_back();
	_playerSegmentIndices.push_back(0);

	_track.setLength(5000);
	_track.setSeed(1);
	_track.generate();
}


void World::handleEvent(const sf::Event& e)
{

}

void World::update(float dt)
{
	for (unsigned i = 0; i < _players.size(); ++i)
	{
		// Finds forward vector of ship and updates segment index
		glm::vec3 forward = _track.findForward(_players[i].getShip().getPosition(), _playerSegmentIndices[i]);
		_players[i].getShip().setForward(forward);

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

	_camera.setPos(glm::vec3{ _players[0].getShip().getTransformMatrix() * glm::vec4{ 0, 2, -12, 1 } });
	_camera.setUp(glm::vec3{ _players[0].getShip().getTransformMatrix() * glm::vec4{ 0, 1, 0, 0 } });
	_camera.setViewDir(glm::vec3{ _players[0].getShip().getTransformMatrix() * glm::vec4{ 0, 0, 1, 0 } });

}

void World::render()
{
	for (Player& player : _players)
	{
		//player.render(_renderer);
		_renderer.render(player.getShip());
	}

	_track.render(_renderer);
	//_renderer.render(_track.);

	PointLight testLight(_players[0].getShip().getPosition(), { 1.f,0.5f,0.f }, 1.f);
	_renderer.pushPointLight(testLight);

	for (int i = 0; i < _pointLights.size(); i++)
	{
		_renderer.pushPointLight(_pointLights[i]);
	}

	_renderer.display(_camera);

	GFX::SfmlRenderer sfml;
	for (Player& player : _players)
	{
		sfml.render(player.getHud());
	}

	sfml.display(_context.window);
}

bool World::bHasWon()
{
	return _bHasWon;
}
