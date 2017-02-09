#include "SFML/Window/Event.hpp"

#include "World.hpp"
#include "ApplicationContext.hpp"
#include "../GFX/SfmlRenderer.hpp"
#include "Track/SegmentInstance.hpp"


World::World(ApplicationContext& context)  
	: _segmentHandler{ "Segments/segmentinfos1.txt", "Segments/ConnectionTypes.txt" }, _track{ &_segmentHandler }, _context{ context }, _camera{ 90.0f, 1280, 720, glm::vec3{0,0,0}, glm::vec3{0,0,1} }
{
	_renderer.initialize(&context.window, 0.0f, 0.0f, 1.0f, 1.0f);

	// Create one player
	_players.emplace_back();
	_playerSegmentIndices.push_back(0);

	_track.setLength(100);
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
		for (long j = static_cast<long>(_playerSegmentIndices[i]) - 1; j <= static_cast<long>(_playerSegmentIndices[i]) + 1; ++j)
		{
			if (j >= 0 && j < _track.getNrOfSegments())
			{
				instances.push_back(_track.getInstance(static_cast<int>(j)));
			}
		}

		// Set relevant segments
		_players[i].getShip().setSegments(instances);

		_players[i].update(dt);
	}

	_camera.setPos(glm::vec3{ _players[0].getShip().getTransformMatrix() * glm::vec4{ 0, 1, -6, 1 } });
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

	PointLight testLight(_players[0].getShip().getPosition(), { 1,1,1 }, 1000.f);
	_renderer.pushPointLight(testLight);
	_renderer.display(_camera);

	GFX::SfmlRenderer sfml;
	for (Player& player : _players)
	{
		sfml.render(player.getHud());
	}

	sfml.display(_context.window);
}
