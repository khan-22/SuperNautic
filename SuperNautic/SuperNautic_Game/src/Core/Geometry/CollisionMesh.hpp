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

    // Generate collision mesh from an OBB
    CollisionMesh(const BoundingBox& obb);

    // Generate collision mesh from a collection of OBBs
    CollisionMesh(const std::vector<BoundingBox>& obbs);

    // Generate collision mesh from a sphere
    CollisionMesh(const Sphere& sphere);

    // Generate collision mesh from an axis aligned plane
    CollisionMesh(const AxisAlignedPlane& axisAlignedPlane);

    // Evaluate collision between this and another collision mesh
    CollisionResult testCollision(const CollisionMesh& other) const;

private:
    enum class Type : unsigned char
    {
        OBB,
        SPHERE,
        AXIS_ALIGNED_PLANE,
        OBB_COLLECTION
    };

    std::vector<Sphere>             _spheres;
    std::vector<BoundingBox>        _obbs;
    AxisAlignedPlane                _axisAlignedPlane;
    Type                            _type;

    // OBB-OBB collision check
    static CollisionResult testCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2);

    // OBB-sphere collision check
    static CollisionResult testCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere);

    // OBB-AxisAlignedPlane collision check
    static CollisionResult testCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane);

    // Sphere-Sphere collision check
    static CollisionResult testCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2);

    // Sphere-AxisAlignedPlane collision check
    static CollisionResult testCollisionSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane);

    // AxisAlignedPlane-AxisAlignedPlane collision check
    static CollisionResult testCollisionAxisAlignedPlaneAxisAlignedPlane(const CollisionMesh& axisAlignedPlane1, const CollisionMesh& axisAlignedPlane2);


    // Obb-ObbCollection collision check
    static CollisionResult testCollisionObbObbCollection(const CollisionMesh& obb, const CollisionMesh& obbCollection);

    // Sphere-ObbCollection collision check
    static CollisionResult testCollisionSphereObbCollection(const CollisionMesh& sphere, const CollisionMesh& obbCollection);

    // AxisAlignedPlane-ObbCollection collision check
    static CollisionResult testCollisionAxisAlignedPlaneObbCollection(const CollisionMesh& axisAlignedPlane, const CollisionMesh& obbCollection);

    // ObbCollection-ObbCollection collision check
    static CollisionResult testCollisionObbCollectionObbCollection(const CollisionMesh& c1, const CollisionMesh& c2);


    // Convert PlaneCollisionData from CollisionUtility functions to
    // CollisionResult data of this class.
    static CollisionResult convertPlaneCollisionDataToCollisionResult(const PlaneCollisionData& data);
};


#endif //COLLISION_MESH_HPP
