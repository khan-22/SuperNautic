#pragma once

#ifndef WORLD_HPP
#define WORLD_HPP

namespace sf
{
    class Event;
}

#include <list>


#include "Player.hpp"
class ApplicationContext;

class World
{
public:
    World(ApplicationContext& context);

	void handleEvent(const sf::Event& e);
	void update(float dt);
	void render();

private:
    std::list<Player> _players;
    ApplicationContext& _context;
};

#endif //WORLD_HPP
