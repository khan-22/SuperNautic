#include "glm/glm.hpp"

#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/BoundingBox.hpp"
#include "Core/Geometry/AxisAlignedPlane.hpp"
#include "Core/Io/Log.hpp"
#include "Core/Utility/Utilities.hpp"

bool bIsSeparatingAxis(const glm::vec3& axis, const BoundingBox& a, const BoundingBox& b);
float computeHalfProjectionLength(const glm::vec3& projectionLine, const BoundingBox& obb);
void assertObbCorrectness(const BoundingBox& obb);
void assertSphereCorrectness(const Sphere& sphere);


bool bTestCollision(const Sphere& a, const Sphere& b)
{
    #ifndef NDEBUG
    assertSphereCorrectness(a);
    assertSphereCorrectness(b);
    #endif // NDEBUG

    glm::vec3 distanceVector = a.center - b.center;
    float distanceSqrd = glm::dot(distanceVector, distanceVector);
    float minDistanceSqrd = a.radius + b.radius;
    minDistanceSqrd *= minDistanceSqrd;
    return distanceSqrd < minDistanceSqrd;
}

bool bTestCollision(const BoundingBox& a, const BoundingBox& b)
{
    #ifndef NDEBUG
    assertObbCorrectness(a);
    assertObbCorrectness(b);
    #endif // NDEBUG

    // Check for separating axis using a's and b's directions.
    for(unsigned char i = 0; i < 3; i++)
    {
        if(bIsSeparatingAxis(a.directions[i], a, b))
        {
            return false;
        }
    }
    for(unsigned char i = 0; i < 3; i++)
    {
        if(bIsSeparatingAxis(b.directions[i], a, b))
        {
            return false;
        }
    }


    // Check for separating axis using all cross products
    // of a's and b's directions.
    for(unsigned char i = 0; i < 3; i++)
    {
        for(unsigned char j = 0; j < 3; j++)
        {
            if(bIsSeparatingAxis(glm::normalize(glm::cross(a.directions[i], b.directions[j])), a, b))
            {
                return false;
            }
        }
    }

    return true;
}

bool bTestCollision(const BoundingBox& obb, const Sphere& sphere)
{
    #ifndef NDEBUG
    assertObbCorrectness(obb);
    assertSphereCorrectness(sphere);
    #endif // NDEBUG

    glm::vec3 distance = sphere.center - obb.center;
    float projectionLengthDistance = glm::length(distance);
    if(floatEq(projectionLengthDistance, 0.f))
    {
        return true;
    }
    glm::vec3 separatingAxis = glm::normalize(distance);
    float projectionLengthObb = computeHalfProjectionLength(separatingAxis, obb);
    float projectionLengthSphere = sphere.radius;

    // Allow equality for edge intersections.
    return !(projectionLengthDistance > projectionLengthObb + projectionLengthSphere);
}

PlaneCollisionData bTestCollision(const BoundingBox& obb, const AxisAlignedPlane& axisAlignedPlane)
{
    #ifndef NDEBUG
    assertObbCorrectness(obb);
    #endif // NDEBUG

    unsigned char axisIndex = axisAlignedPlane.getAxisIndex();

    float projectionLengthDistance = std::fabs(axisAlignedPlane.distance - obb.center[axisIndex]);
    float projectionLengthObb = 0.f;
    for(unsigned char i = 0; i < 3; i++)
    {
        projectionLengthObb += std::fabs(obb.halfLengths[i] * obb.directions[i][axisIndex]);
    }

    PlaneCollisionData data;
    if(projectionLengthDistance > projectionLengthObb)
    {
        if(obb.center[axisIndex] > axisAlignedPlane.distance)
        {
            data.type = PlaneCollisionData::Type::FRONT;
        }
        else
        {
            data.type = PlaneCollisionData::Type::BACK;
        }
        return data;
    }

    data.type = PlaneCollisionData::Type::COLLISION;
    return data;
}

PlaneCollisionData bTestCollision(const Sphere& sphere, const AxisAlignedPlane& axisAlignedPlane)
{
    #ifndef NDEBUG
    assertSphereCorrectness(sphere);
    #endif // NDEBUG

    float spherePos = sphere.center[axisAlignedPlane.getAxisIndex()];

    PlaneCollisionData data;
    if(spherePos - sphere.radius > axisAlignedPlane.distance)
    {
        data.type = PlaneCollisionData::Type::FRONT;
    }
    else if(spherePos + sphere.radius < axisAlignedPlane.distance)
    {
        data.type = PlaneCollisionData::Type::BACK;
    }
    else
    {
        data.type = PlaneCollisionData::Type::COLLISION;
    }

    return data;
}

PlaneCollisionData bTestCollision(const AxisAlignedPlane& axisAlignedPlane1, const AxisAlignedPlane& axisAlignedPlane2)
{
    PlaneCollisionData data;
    if(axisAlignedPlane1.axis == axisAlignedPlane2.axis)
    {
        if(axisAlignedPlane1.distance > axisAlignedPlane2.distance)
        {
            data.type == PlaneCollisionData::Type::FRONT;
        }
        else if(axisAlignedPlane1.distance < axisAlignedPlane2.distance)
        {
            data.type = PlaneCollisionData::Type::BACK;
        }
        else
        {
            data.type == PlaneCollisionData::Type::COLLISION;
        }
    }
    else
    {
        data.type == PlaneCollisionData::Type::COLLISION;
    }

    return data;
}


bool bIsSeparatingAxis(const glm::vec3& axis, const BoundingBox& a, const BoundingBox& b)
{
    if(!std::isfinite(axis.x) || !std::isfinite(axis.y) || !std::isfinite(axis.z))
    {
        return false;
    }

    if(floatEq(std::fabs(axis.x) + std::fabs(axis.y) + std::fabs(axis.z), 0.f))
    {
        return false;
    }

    #ifndef NDEBUG
    assert(floatEq(glm::length(axis), 1.f));
    assertObbCorrectness(a);
    assertObbCorrectness(b);
    #endif // NDEBUG

    float projectionLengthDistance = std::fabs(glm::dot(b.center - a.center, axis));
    float projectionLengthA = computeHalfProjectionLength(axis, a);
    float projectionLengthB = computeHalfProjectionLength(axis, b);
    return projectionLengthDistance > projectionLengthA + projectionLengthB;
}

float computeHalfProjectionLength(const glm::vec3& projectionLine, const BoundingBox& obb)
{

    #ifndef NDEBUG
    if(!floatEq(glm::length(projectionLine), 1.f))
    {
        int flerp = 0;
    }
    assert(floatEq(glm::length(projectionLine), 1.f));
    assertObbCorrectness(obb);
    #endif // NDEBUG

    float length = 0.f;
    for(unsigned char i = 0; i < 3; i++)
    {
        length += obb.halfLengths[i] * std::fabs(glm::dot(obb.directions[i], projectionLine));
    }

    return length;
}

void assertObbCorrectness(const BoundingBox& obb)
{
    for(unsigned char i = 0; i < 3; i++)
    {
        assert(floatEq(glm::length(obb.directions[i]), 1.f));
        assert(obb.halfLengths[i] > 0.f);
    }
}

void assertSphereCorrectness(const Sphere& sphere)
{
    assert(sphere.radius > 0.f);
}
