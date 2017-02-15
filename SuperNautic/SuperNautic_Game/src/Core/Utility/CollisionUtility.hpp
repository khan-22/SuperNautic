#pragma once

#ifndef COLLISION_UTILITY_HPP
#define COLLISION_UTILITY_HPP

struct Sphere;
struct BoundingBox;
struct AxisAlignedPlane;

struct PlaneCollisionData
{
    enum class Type : unsigned char
    {
        COLLISION, // Intersection.
        FRONT, // LHS is in front of RHS.
        BACK, // LHS is behind RHS.
    };

    Type type;
};

bool bTestCollision(const Sphere& a, const Sphere& b);
bool bTestCollision(const BoundingBox& a, const BoundingBox& b);
bool bTestCollision(const BoundingBox& obb, const Sphere& sphere);
PlaneCollisionData bTestCollision(const BoundingBox& obb, const AxisAlignedPlane& axisAlignedPlane);
PlaneCollisionData bTestCollision(const Sphere& sphere, const AxisAlignedPlane& axisAlignedPlane);
PlaneCollisionData bTestCollision(const AxisAlignedPlane& axisAlignedPlane1, const AxisAlignedPlane& axisAlignedPlane2);

#endif //COLLISION_UTILITY_HPP
