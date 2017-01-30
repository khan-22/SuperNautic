#include "Ship.hpp"


Ship::Ship()
: _steerVelocity(0.f)
, _forwardVelocity(0.f)
, _bIsJumping(false)
, _maxSteerAcceleration(2.f)
, _maxForwardAcceleration(10.f)
, _engineTemperature(0.f)
{

}

void Ship::render()
{
    // ...
}

void Ship::updateEngineTemperature(float dt)
{
    // Placeholder.. Should probably not be linear. :-)
    // tempIncrease(speed) = K * speed + M
    static const float K = 1.5f;
    static const float BREAK_EVEN_SPEED = 10.f;
    static const float M = K * BREAK_EVEN_SPEED;
    static const float K_NORMALIZED = K / 100.f;
    static const float M_NORMALIZED = M / 100.f;
    _engineTemperature += (_forwardVelocity * K_NORMALIZED - M_NORMALIZED) * dt;

    if(_engineTemperature > 1.f)
    {
        _engineTemperature = 1.f;
    }
    else if(_engineTemperature < 0.f)
    {
        _engineTemperature = 0.f;
    }
}

void Ship::update(float dt)
{

    updateEngineTemperature(dt);

    // Need racing track segment info...
    glm::vec3 forward(1.f, 0.f, 0.f);
    forward *= _forwardVelocity;


    glm::vec3 left(0.f, 1.f, 0.f);
    left *= _steerVelocity;

    move((forward + left) * dt);
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

float Ship::getEngineTemperature() const
{
    return _engineTemperature;
}

float Ship::getSpeed() const
{
    return _forwardVelocity;
}
