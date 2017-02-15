#pragma once

#ifndef COLLISION_MESH_HPP
#define COLLISION_MESH_HPP

#include "Core/Geometry/BoundingBox.hpp"
#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/AxisAlignedPlane.hpp"

struct PlaneCollisionData;

class CollisionMesh
{
public:
    enum class CollisionResult : unsigned char
    {
        COLLISION, // Collision occurred.
        NO_COLLISION, // No collision occurred.
        FRONT,  // No collision occurred. LHS (this) is in front of RHS.
        BACK, // No collision occurred. LHS (this) is behind of RHS.
    };

    CollisionMesh(const BoundingBox& obb);
    CollisionMesh(const Sphere& sphere);
    CollisionMesh(const AxisAlignedPlane& axisAlignedPlane);

    CollisionResult testCollision(const CollisionMesh& other) const;

private:
    enum class Type : unsigned char
    {
        OBB,
        SPHERE,
        AXIS_ALIGNED_PLANE,
    };

    Sphere _sphere;
    BoundingBox _obb;
    AxisAlignedPlane _axisAlignedPlane;
    Type _type;

    static CollisionResult testCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2);
    static CollisionResult testCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere);
    static CollisionResult testCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane);
    static CollisionResult testCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2);
    static CollisionResult testCollisionSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane);
    static CollisionResult testCollisionAxisAlignedPlaneAxisAlignedPlane(const CollisionMesh& axisAlignedPlane1, const CollisionMesh& axisAlignedPlane2);
    static CollisionResult convertPlaneCollisionDataToCollisionResult(const PlaneCollisionData& data);
};


#endif //COLLISION_MESH_HPP
