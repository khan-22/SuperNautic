#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "World.hpp"
#include "ApplicationContext.hpp"


World::World(ApplicationContext& context)
: _context(context)
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

    for(Player& player : _players)
    {
        player.renderHUD(_context.window, sf::RenderStates::Default);
    }
}
