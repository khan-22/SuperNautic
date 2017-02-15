#pragma once

#ifndef COLLISION_UTILITY_HPP
#define COLLISION_UTILITY_HPP

struct Sphere;
struct BoundingBox;

bool bTestCollision(const Sphere& a, const Sphere& b);
bool bTestCollision(const BoundingBox& a, const BoundingBox& b);

#endif //COLLISION_UTILITY_HPP
