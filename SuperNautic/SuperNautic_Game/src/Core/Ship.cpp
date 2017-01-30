#include "Ship.hpp"


Ship::Ship()
: _steerVelocity(0.f)
, _forwardVelocity(0.f)
, _bIsJumping(false)
, _maxSteerAcceleration(2.f)
, _maxForwardAcceleration(10.f)
{

}

void Ship::render()
{
    // ...
}

void Ship::update(float dt)
{
    // Need racing track segment info...
    glm::vec3 forward(1.f, 0.f, 0.f);
    glm::vec3 left(0.f, 1.f, 0.f);
    move((forward * _forwardVelocity + left * _steerVelocity) * dt);
}

void Ship::steer(float magnitude)
{
    _steerVelocity += magnitude * _maxSteerAcceleration;
}

void Ship::accelerate(float magnitude)
{
    _forwardVelocity += magnitude * _maxForwardAcceleration;
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
