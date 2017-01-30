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

	void steer(float magnitude);
	void accelerate(float magnitude);
	void jump();

private:
    float _steerVelocity;
    float _forwardVelocity;
    bool _bIsJumping;
};

#endif // SHIP_HPP
