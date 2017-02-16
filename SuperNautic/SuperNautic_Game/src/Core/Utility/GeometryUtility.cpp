#include <cmath>

#include "Core/Utility/GeometryUtility.hpp"




Sphere generateBoundingSphere(const BoundingBox& obb)
{
    Sphere sphere;

    sphere.center = obb.center;

    sphere.radius = 0.f;
    for(const float& halfLength : obb.halfLengths)
    {
        sphere.radius += halfLength * halfLength;
    }
    sphere.radius = std::sqrt(sphere.radius);

    return sphere;
}
