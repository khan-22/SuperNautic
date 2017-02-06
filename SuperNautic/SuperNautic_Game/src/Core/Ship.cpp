#include <math.h>

#include "Ship.hpp"


Ship::Ship()
{

}

Ship::Ship(glm::vec3 position)
{
	setPosition(position);
}

void Ship::render()
{
    // ...
}



void Ship::update(float dt)
{

	
	// Reset values to stop turning/acceleration if no input is provided
	_turningFactor = 0.0f;
	_accelerationFactor = 0.5f;
}
