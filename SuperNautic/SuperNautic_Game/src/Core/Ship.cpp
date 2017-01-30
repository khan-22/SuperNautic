#include "Ship.hpp"


Ship::Ship()
: _steerVelocity(0.f)
, _forwardVelocity(0.f)
, _bIsJumping(false)
{

}

void Ship::render()
{
    // ...
}

void Ship::update(float dt)
{
    // Need racing track segment info...
    move(glm::vec3());
}

void Ship::steer(float magnitude)
{
    _steerVelocity += magnitude;
}

void Ship::accelerate(float magnitude)
{
    _forwardVelocity += magnitude;
}

void Ship::jump()
{
    if(_bIsJumping)
    {
        // Need communication with racing track segment...
        return;
    }

    _bIsJumping = true;
}
