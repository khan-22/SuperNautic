#include "Core/Geometry/CollisionMesh.hpp"
#include "Core/Utility/GeometryUtility.hpp"


CollisionMesh::CollisionMesh(const BoundingBox& obb)
: _obb(obb)
, _sphere(generateBoundingSphere(obb))
, _type(Type::OBB)
{

}

CollisionMesh::CollisionMesh(const Sphere& sphere)
: _sphere(sphere)
, _type(Type::SPHERE)
{

}

CollisionMesh::CollisionMesh(const AxisAlignedPlane& axisAlignedPlane)
: _axisAlignedPlane(axisAlignedPlane)
, _type(Type::AXIS_ALIGNED_PLANE)
{

}

bool CollisionMesh::bTestCollision(const CollisionMesh& other) const
{
    return true;
}
