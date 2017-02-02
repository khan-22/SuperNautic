#include <math.h>

#include "Ship.hpp"


Ship::Ship()
: _steerVelocity(0.f)
, _forwardVelocity(0.f)
, _bIsJumping(false)
, _maxSteerAcceleration(2.f)
, _maxForwardAcceleration(50.f)
, _engineTemperature(0.f)
, _maxSteerVelocity(10.f)
, _maxForwardVelocity(100.f)
, _minForwardVelocity(20.f)
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
    static const float BREAK_EVEN_SPEED = 50.f;
    static const float M = K * BREAK_EVEN_SPEED;
    static const float K_NORMALIZED = K / 100.f;
    static const float M_NORMALIZED = M / 100.f;
	_engineTemperature = (_forwardVelocity) / _maxForwardVelocity;

    if(_engineTemperature > 1.f)
    {
        _engineTemperature = 1.f;
    }
    else if(_engineTemperature < 0.f)
    {
        _engineTemperature = 0.f;
    }
}

void Ship::updateVelocities(float dt)
{
    _steerVelocity -= _steerVelocity * 0.1f * dt;
    //_forwardVelocity -= _forwardVelocity * 0.1f * dt;
    if(_steerVelocity > _maxSteerVelocity)
    {
        _steerVelocity = _maxSteerVelocity;
    }
    else if(_steerVelocity < -_maxSteerVelocity)
    {
        _steerVelocity = -_maxSteerVelocity;
    }

    if(_forwardVelocity > _maxForwardVelocity)
    {
        _forwardVelocity = _maxForwardVelocity;
    }
    else if(_forwardVelocity < _minForwardVelocity)
    {
        _forwardVelocity = _minForwardVelocity;
    }
}

void Ship::update(float dt)
{

    updateEngineTemperature(dt);
    updateVelocities(dt);


    // Need racing track segment info...
    glm::vec3 forward(1.f, 0.f, 0.f);
    forward *= _forwardVelocity;

    glm::vec3 right(0.f, 1.f, 0.f);
    right *= _steerVelocity;

    move((forward + right) * dt);
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
