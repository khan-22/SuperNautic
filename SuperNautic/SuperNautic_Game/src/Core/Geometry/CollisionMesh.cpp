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

CollisionMesh::CollisionResult CollisionMesh::testCollision(const CollisionMesh& other) const
{
    switch(_type)
    {
    case Type::OBB:
        switch(other._type)
        {
        case Type::OBB: return testCollisionObbObb(*this, other);
        case Type::SPHERE: return testCollisionObbSphere(*this, other);
        case Type::AXIS_ALIGNED_PLANE: return testCollisionObbAxisAlignedPlane(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return CollisionResult::NO_COLLISION;
        }
        break;
    case Type::SPHERE:
        switch(other._type)
        {
        case Type::OBB: return testCollisionObbSphere(other, *this);
        case Type::SPHERE: return testCollisionSphereSphere(*this, other);
        case Type::AXIS_ALIGNED_PLANE: return testCollisionSphereAxisAlignedPlane(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return CollisionResult::NO_COLLISION;
        }
        break;
    case Type::AXIS_ALIGNED_PLANE:
        switch(other._type)
        {
        case Type::OBB: return testCollisionObbAxisAlignedPlane(other, *this);
        case Type::SPHERE: return testCollisionSphereAxisAlignedPlane(other, *this);
        case Type::AXIS_ALIGNED_PLANE: return testCollisionAxisAlignedPlaneAxisAlignedPlane(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return CollisionResult::NO_COLLISION;
        }
        break;

    default:
        LOG_ERROR("Unimplemented CollisionMesh type: ", (int)_type);
        return CollisionResult::NO_COLLISION;
    }
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2)
{
    if(!::bTestCollision(obb1._sphere, obb2._sphere))
    {
        return CollisionResult::NO_COLLISION;
    }

    if(!::bTestCollision(obb1._obb, obb2._obb))
    {
        return CollisionResult::NO_COLLISION;
    }

    return CollisionResult::COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere)
{
    if(!::bTestCollision(obb._sphere, sphere._sphere))
    {
        return CollisionResult::NO_COLLISION;
    }

    if(!::bTestCollision(obb._obb, sphere._sphere))
    {
        return CollisionResult::NO_COLLISION;
    }

    return CollisionResult::COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane)
{
    CollisionResult result = convertPlaneCollisionDataToCollisionResult(::testCollision(obb._sphere, axisAlignedPlane._axisAlignedPlane));

    if(result != CollisionResult::COLLISION)
    {
        return result;
    }

    return convertPlaneCollisionDataToCollisionResult(::testCollision(obb._obb, axisAlignedPlane._axisAlignedPlane));
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2)
{
    if(!::bTestCollision(sphere1._sphere, sphere2._sphere))
    {
        return CollisionResult::NO_COLLISION;
    }

    return CollisionResult::COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane)
{
    return convertPlaneCollisionDataToCollisionResult(::testCollision(sphere._sphere, axisAlignedPlane._axisAlignedPlane));
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionAxisAlignedPlaneAxisAlignedPlane(const CollisionMesh& axisAlignedPlane1, const CollisionMesh& axisAlignedPlane2)
{
    return convertPlaneCollisionDataToCollisionResult(::testCollision(axisAlignedPlane1._axisAlignedPlane, axisAlignedPlane2._axisAlignedPlane));
}

CollisionMesh::CollisionResult CollisionMesh::convertPlaneCollisionDataToCollisionResult(const PlaneCollisionData& data)
{
    switch(data.type)
    {
        case PlaneCollisionData::Type::FRONT: return CollisionResult::FRONT;
        case PlaneCollisionData::Type::BACK: return CollisionResult::BACK;
        case PlaneCollisionData::Type::COLLISION: return CollisionResult::COLLISION;

        default:
            LOG_ERROR("Unimplemented plane collision data type: ", (int)data.type);
            return CollisionResult::NO_COLLISION;
    }
}
