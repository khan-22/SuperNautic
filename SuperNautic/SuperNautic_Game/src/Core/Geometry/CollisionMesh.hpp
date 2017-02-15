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
};


#endif //COLLISION_MESH_HPP
