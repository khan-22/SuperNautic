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
	void updatePower(float magnitude); // magnitude = [-1, 1]
	void jump();

	float getEngineTemperature() const; // [0, 1]
	float getSpeed() const;

private:
	float _enginePower;
	float _maxEnginePower;
    float _steerVelocity;
    float _forwardVelocity;
    float _maxSteerAcceleration;
    float _maxForwardAcceleration;
    float _maxSteerVelocity;
    float _maxForwardVelocity;
    float _minForwardVelocity;
    float _engineTemperature; // [0, 1]
    bool _bIsJumping;

    void updateEngineTemperature(float dt);
    void updateVelocities(float dt);


};

#endif // SHIP_HPP
