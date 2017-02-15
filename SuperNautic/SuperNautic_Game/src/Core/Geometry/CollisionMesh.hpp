#pragma once

#ifndef COLLISION_MESH_HPP
#define COLLISION_MESH_HPP

#include "Core/Geometry/BoundingBox.hpp"
#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/AxisAlignedPlane.hpp"

class CollisionMesh
{
public:
    CollisionMesh(const BoundingBox& obb);
    CollisionMesh(const Sphere& sphere);
    CollisionMesh(const AxisAlignedPlane& axisAlignedPlane);

    bool bTestCollision(const CollisionMesh& other) const;

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

    static bool bTestCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2);
    static bool bTestCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere);
    static bool bTestCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane);
    static bool bTestCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2);
    static bool bTestCollisioSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane);
    static bool bTestCollisionAxisAlignedPlaneAxisAlignedPlane(const CollisionMesh& axisAlignedPlane1, const CollisionMesh& axisAlignedPlane2);
};


#endif //COLLISION_MESH_HPP
