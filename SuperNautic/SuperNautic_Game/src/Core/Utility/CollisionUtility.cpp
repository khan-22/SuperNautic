#include "glm/glm.hpp"

#include "Core/Utility/CollisionUtility.hpp"
#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/BoundingBox.hpp"

bool bIsSeparatingAxis(const glm::vec3& axis, const BoundingBox& a, const BoundingBox& b);


bool bTestCollision(const Sphere& a, const Sphere& b)
{
    glm::vec3 distanceVector = a.center - b.center;
    float distanceSqrd = glm::dot(distanceVector, distanceVector);

    float maxRadius = std::max(a.radius, b.radius);

    return distanceSqrd < maxRadius * maxRadius;
}

bool bIsSeparatingAxis(const glm::vec3& axis, const BoundingBox& a, const BoundingBox& b)
{
    float projectionLengthA = 0.f;
    float projectionLengthB = 0.f;
    for(unsigned char i = 0; i < 3; i++)
    {
        projectionLengthA += a.halfLengths[i] * std::fabs(glm::dot(a.directions[i], axis));
        projectionLengthB += b.halfLengths[i] * std::fabs(glm::dot(b.directions[i], axis));
    }

    float projectionLengthDistance = std::fabs(glm::dot(b.center - a.center, axis));

    return projectionLengthDistance > projectionLengthA + projectionLengthB;
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
