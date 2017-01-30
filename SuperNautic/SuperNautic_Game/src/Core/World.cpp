#include "SFML/Window/Event.hpp"

#include "World.hpp"

World::World()
{
    _players.emplace_back();
}


void World::handleEvent(const sf::Event& e)
{

}

void World::update(float dt)
{
    for(Player& player : _players)
    {
        player.update(dt);
    }
}

void World::render()
{
    for(Player& player : _players)
    {
        player.render();
    }
}
