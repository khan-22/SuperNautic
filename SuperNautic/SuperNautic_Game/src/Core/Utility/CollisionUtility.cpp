#include "glm/glm.hpp"

#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/BoundingBox.hpp"

bool bIsSeparatingAxis(const glm::vec3& axis, const BoundingBox& a, const BoundingBox& b);
float computeHalfProjectionLength(const glm::vec3& projectionLine, const BoundingBox& obb);


bool bTestCollision(const Sphere& a, const Sphere& b)
{
    glm::vec3 distanceVector = a.center - b.center;
    float distanceSqrd = glm::dot(distanceVector, distanceVector);

    float maxRadius = std::max(a.radius, b.radius);

    return distanceSqrd < maxRadius * maxRadius;
}

bool bTestCollision(const BoundingBox& a, const BoundingBox& b)
{
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
            if(bIsSeparatingAxis(glm::cross(a.directions[i], b.directions[j]), a, b))
            {
                return false;
            }
        }
    }

    return true;
}

bool bTestCollision(const BoundingBox& obb, const Sphere& sphere)
{
    glm::vec3 distance = sphere.center - obb.center;
    float projectionLengthDistance = glm::length(distance);

    glm::vec3 separatingAxis = glm::normalize(distance);
    float projectionLengthObb = computeHalfProjectionLength(separatingAxis, obb);
    float projectionLengthSphere = sphere.radius;

    return projectionLengthDistance > projectionLengthObb + projectionLengthSphere;
}


bool bIsSeparatingAxis(const glm::vec3& axis, const BoundingBox& a, const BoundingBox& b)
{
    float projectionLengthDistance = std::fabs(glm::dot(b.center - a.center, axis));
    float projectionLengthA = computeHalfProjectionLength(axis, a);
    float projectionLengthB = computeHalfProjectionLength(axis, b);
    return projectionLengthDistance > projectionLengthA + projectionLengthB;
}

float computeHalfProjectionLength(const glm::vec3& projectionLine, const BoundingBox& obb)
{
    float length = 0.f;
    for(unsigned char i = 0; i < 3; i++)
    {
        length += obb.halfLengths[i] * std::fabs(glm::dot(obb.directions[i], projectionLine));
    }

    return length;
}
