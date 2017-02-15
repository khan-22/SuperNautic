#include "Core/Geometry/CollisionMesh.hpp"
#include "Core/Utility/GeometryUtility.hpp"
#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Io/Log.hpp"

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
    switch(_type)
    {
    case Type::OBB:
        switch(other._type)
        {
        case Type::OBB: return bTestCollisionObbObb(*this, other);
        case Type::SPHERE: return bTestCollisionObbSphere(*this, other);
        case Type::AXIS_ALIGNED_PLANE: return bTestCollisionObbAxisAlignedPlane(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return false;
        }
        break;
    case Type::SPHERE:
        switch(other._type)
        {
        case Type::OBB: return bTestCollisionObbSphere(other, *this);
        case Type::SPHERE: return bTestCollisionSphereSphere(*this, other);
        case Type::AXIS_ALIGNED_PLANE: return bTestCollisioSphereAxisAlignedPlane(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return false;
        }
        break;
    case Type::AXIS_ALIGNED_PLANE:
        switch(other._type)
        {
        case Type::OBB: return bTestCollisionObbAxisAlignedPlane(other, *this);
        case Type::SPHERE: return bTestCollisioSphereAxisAlignedPlane(other, *this);
        case Type::AXIS_ALIGNED_PLANE: return bTestCollisionAxisAlignedPlaneAxisAlignedPlane(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return false;
        }
        break;

    default:
        LOG_ERROR("Unimplemented CollisionMesh type: ", (int)_type);
        return false;
    }
}

bool CollisionMesh::bTestCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2)
{
    if(!::bTestCollision(obb1._sphere, obb2._sphere))
    {
        return false;
    }

    return ::bTestCollision(obb1._obb, obb2._obb);
}

bool CollisionMesh::bTestCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere)
{
    if(!::bTestCollision(obb._sphere, sphere._sphere))
    {
        return false;
    }

    return ::bTestCollision(obb._obb, sphere._sphere);
}

bool CollisionMesh::bTestCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane)
{
    if(!::bTestCollision(obb._sphere, axisAlignedPlane._axisAlignedPlane))
    {
        return false;
    }

    return ::bTestCollision(obb._obb, axisAlignedPlane._axisAlignedPlane);
}

bool CollisionMesh::bTestCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2)
{
    return ::bTestCollision(sphere1._sphere, sphere2._sphere);
}

bool CollisionMesh::bTestCollisioSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane)
{
    return ::bTestCollision(sphere._sphere, axisAlignedPlane._axisAlignedPlane);
}

bool CollisionMesh::bTestCollisionAxisAlignedPlaneAxisAlignedPlane(const CollisionMesh& axisAlignedPlane1, const CollisionMesh& axisAlignedPlane2)
{
    return ::bTestCollision(axisAlignedPlane1._axisAlignedPlane, axisAlignedPlane1._axisAlignedPlane);
}

