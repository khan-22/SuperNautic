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

    CollisionResult bTestCollision(const CollisionMesh& other) const;

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

    static CollisionResult bTestCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2);
    static CollisionResult bTestCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere);
    static CollisionResult bTestCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane);
    static CollisionResult bTestCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2);
    static CollisionResult bTestCollisioSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane);
    static CollisionResult bTestCollisionAxisAlignedPlaneAxisAlignedPlane(const CollisionMesh& axisAlignedPlane1, const CollisionMesh& axisAlignedPlane2);
    static CollisionResult convertPlaneCollisionDataToCollisionResult(const PlaneCollisionData& data);
};


#endif //COLLISION_MESH_HPP
