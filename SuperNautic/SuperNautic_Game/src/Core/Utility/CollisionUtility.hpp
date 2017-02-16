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

// True if spheres intersect
bool bTestCollision(const Sphere& a, const Sphere& b);

// True if OBBs intersect
bool bTestCollision(const BoundingBox& a, const BoundingBox& b);

// True if OBB and sphere intersect
bool bTestCollision(const BoundingBox& obb, const Sphere& sphere);

// Evaluate OBB-AxisAlignedPlane intersection
PlaneCollisionData testCollision(const BoundingBox& obb, const AxisAlignedPlane& axisAlignedPlane);

// Evaluate sphere-AxisAlignedPlane intersection
PlaneCollisionData testCollision(const Sphere& sphere, const AxisAlignedPlane& axisAlignedPlane);

// Evaluate AxisAlignedPlane-AxisAlignedPlane intersection
PlaneCollisionData testCollision(const AxisAlignedPlane& axisAlignedPlane1, const AxisAlignedPlane& axisAlignedPlane2);

#endif //COLLISION_UTILITY_HPP
