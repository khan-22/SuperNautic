#include <set>

#include "Core/Geometry/CollisionMesh.hpp"
#include "Core/Utility/GeometryUtility.hpp"
#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Io/Log.hpp"

CollisionMesh::CollisionMesh(const BoundingBox& obb)
: _obbs({obb})
, _spheres({generateBoundingSphere(obb)})
, _type(Type::OBB)
{

}

CollisionMesh::CollisionMesh(const std::vector<BoundingBox>& obbs)
: _obbs(obbs)
, _type(Type::OBB_COLLECTION)
{
    for(const BoundingBox& obb : obbs)
    {
        _spheres.push_back(generateBoundingSphere(obb));
    }

    _spheres.insert(_spheres.begin(), generateBoundingSphere(_spheres));
}

CollisionMesh::CollisionMesh(const Sphere& sphere)
: _spheres({sphere})
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
        case Type::OBB_COLLECTION: return testCollisionObbObbCollection(*this, other);
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
        case Type::OBB_COLLECTION: return testCollisionSphereObbCollection(*this, other);
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
        case Type::OBB_COLLECTION: return testCollisionAxisAlignedPlaneObbCollection(*this, other);
        default:
            LOG_ERROR("Unimplemented CollisionMesh type: ", (int)other._type);
            return CollisionResult::NO_COLLISION;
        }
        break;

    case Type::OBB_COLLECTION:
        switch(other._type)
        {
        case Type::OBB: return testCollisionObbObbCollection(other, *this);
        case Type::SPHERE: return testCollisionSphereObbCollection(other, *this);
        case Type::AXIS_ALIGNED_PLANE: return testCollisionAxisAlignedPlaneObbCollection(other, *this);
        case Type::OBB_COLLECTION: return testCollisionObbCollectionObbCollection(*this, other);
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

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbObbCollection(const CollisionMesh& obb, const CollisionMesh& obbCollection)
{
    if(!::bTestCollision(obb._spheres[0], obbCollection._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    for(size_t i = 1; i < obbCollection._spheres.size(); i++)
    {
        if(::bTestCollision(obb._spheres[0], obbCollection._spheres[i]) && ::bTestCollision(obb._obbs[0], obbCollection._obbs[i - 1]))
        {
            return CollisionResult::COLLISION;
        }
    }
    return CollisionResult::NO_COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionSphereObbCollection(const CollisionMesh& sphere, const CollisionMesh& obbCollection)
{
    if(!::bTestCollision(sphere._spheres[0], obbCollection._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    for(size_t i = 1; i < obbCollection._spheres.size(); i++)
    {
        if(::bTestCollision(sphere._spheres[0], obbCollection._spheres[i]))
        {
            return CollisionResult::COLLISION;
        }
    }
    return CollisionResult::NO_COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionAxisAlignedPlaneObbCollection(const CollisionMesh& axisAlignedPlane, const CollisionMesh& obbCollection)
{
    CollisionResult result = convertPlaneCollisionDataToCollisionResult(::testCollision(obbCollection._spheres[0], axisAlignedPlane._axisAlignedPlane));
    if(result != CollisionResult::COLLISION)
    {
        return result;
    }

    std::set<CollisionResult> resultsSphere;
    std::set<CollisionResult> resultsObb;
    for(size_t i = 1; i < obbCollection._spheres.size(); i++)
    {
        CollisionResult resultSphere = convertPlaneCollisionDataToCollisionResult(::testCollision(obbCollection._spheres[i], axisAlignedPlane._axisAlignedPlane));
        if(resultSphere == CollisionResult::COLLISION)
        {
            CollisionResult resultObb = convertPlaneCollisionDataToCollisionResult(::testCollision(obbCollection._obbs[i - 1], axisAlignedPlane._axisAlignedPlane));
            if(resultObb == CollisionResult::COLLISION)
            {
                return resultObb;
            }
            resultsObb.insert(resultObb);
        }
        else
        {
            resultsSphere.insert(resultSphere);
        }
    }

    if(!resultsObb.empty())
    {
        if(resultsObb.size() == 1)
        {
            return *resultsObb.begin();
        }
        // If there are OBBs on both sides of the plane, but none of them
        // actually intersect the plane, then consider the OBB collection
        // to be intersecting the plane anyway.
        return CollisionResult::COLLISION;
    }

    if(!resultsSphere.empty())
    {
        if(resultsSphere.size() == 1)
        {
            return *resultsSphere.begin();
        }
        // If there are bounding spheres on both sides of the plane, but none of them
        // actually intersect the plane, then consider the OBB collection
        // to be intersecting the plane anyway.
        return CollisionResult::COLLISION;
    }

    // OBB collection is empty
    return CollisionResult::NO_COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbCollectionObbCollection(const CollisionMesh& c1, const CollisionMesh& c2)
{
    if(!::bTestCollision(c1._spheres[0], c2._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    for(size_t i = 1; i < c1._spheres.size(); i++)
    {
        for(size_t j = i; j < c2._spheres.size(); j++)
        {
            if(::bTestCollision(c1._spheres[i], c2._spheres[j]) && ::bTestCollision(c1._obbs[i - 1], c2._obbs[j - 1]))
            {
                return CollisionResult::COLLISION;
            }
        }
    }
    return CollisionResult::NO_COLLISION;
}


CollisionMesh::CollisionResult CollisionMesh::testCollisionObbObb(const CollisionMesh& obb1, const CollisionMesh& obb2)
{
    if(!::bTestCollision(obb1._spheres[0], obb2._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    if(!::bTestCollision(obb1._obbs[0], obb2._obbs[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    return CollisionResult::COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbSphere(const CollisionMesh& obb, const CollisionMesh& sphere)
{
    if(!::bTestCollision(obb._spheres[0], sphere._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    if(!::bTestCollision(obb._obbs[0], sphere._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    return CollisionResult::COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionObbAxisAlignedPlane(const CollisionMesh& obb, const CollisionMesh& axisAlignedPlane)
{
    CollisionResult result = convertPlaneCollisionDataToCollisionResult(::testCollision(obb._spheres[0], axisAlignedPlane._axisAlignedPlane));

    if(result != CollisionResult::COLLISION)
    {
        return result;
    }

    return convertPlaneCollisionDataToCollisionResult(::testCollision(obb._obbs[0], axisAlignedPlane._axisAlignedPlane));
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionSphereSphere(const CollisionMesh& sphere1, const CollisionMesh& sphere2)
{
    if(!::bTestCollision(sphere1._spheres[0], sphere2._spheres[0]))
    {
        return CollisionResult::NO_COLLISION;
    }

    return CollisionResult::COLLISION;
}

CollisionMesh::CollisionResult CollisionMesh::testCollisionSphereAxisAlignedPlane(const CollisionMesh& sphere, const CollisionMesh& axisAlignedPlane)
{
    return convertPlaneCollisionDataToCollisionResult(::testCollision(sphere._spheres[0], axisAlignedPlane._axisAlignedPlane));
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
