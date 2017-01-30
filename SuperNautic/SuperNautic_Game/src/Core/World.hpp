#pragma once

#ifndef WORLD_HPP
#define WORLD_HPP

namespace sf
{
    class Event;
}

#include <list>


#include "Player.hpp"

class World
{
public:
    World();

	void handleEvent(const sf::Event& e);
	void update(float dt);
	void render();

private:
    std::list<Player> _players;
};

#endif //WORLD_HPP
