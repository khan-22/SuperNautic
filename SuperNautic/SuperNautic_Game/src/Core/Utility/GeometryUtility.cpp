#include <cmath>
#include <algorithm>


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



Sphere generateBoundingSphere(const std::vector<Sphere>& spheres)
{
    Sphere sphere;
    sphere.center = glm::vec3(0.f, 0.f, 0.f);
    sphere.radius = 0.f;
    if(spheres.empty())
    {
        return sphere;
    }

    sphere.center = std::accumulate(spheres.begin(), spheres.end(), glm::vec3(0.f, 0.f, 0.f), [](const glm::vec3& sum, const Sphere& s)
    {
        return sum + s.center;
    });
    sphere.center /= float(spheres.size());

    std::vector<float> distances(spheres.size());
    std::transform(spheres.begin(), spheres.end(), distances.begin(), [&sphere](const Sphere& s)
    {
        return glm::distance(sphere.center, s.center) + s.radius;
    });

    sphere.radius = *std::max_element(distances.begin(), distances.end());
    return sphere;
}
