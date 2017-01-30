#pragma once

#ifndef SHIP_HPP
#define SHIP_HPP

#include "../GFX/Transformable3D.hpp"

class Ship : public GFX::Transformable3D
{
public:
	Ship();

	void render();

	void update(float dt);

	void steer(float magnitude); // magnitude = [-1, 1]
	void accelerate(float magnitude); // magnitude = [-1, 1]
	void jump();

private:
    float _steerVelocity;
    float _forwardVelocity;
    float _maxSteerAcceleration;
    float _maxForwardAcceleration;
    float _engineTemperature; // [0, 1]
    bool _bIsJumping;

    void updateEngineTemperature(float dt);


};

#endif // SHIP_HPP
