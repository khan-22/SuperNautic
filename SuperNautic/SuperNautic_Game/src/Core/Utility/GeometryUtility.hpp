#pragma once

#ifndef GEOMETRY_UTILITY_HPP
#define GEOMETRY_UTILITY_HPP

#include <vector>

#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/BoundingBox.hpp"

// Generate a bounding sphere for obb.
Sphere generateBoundingSphere(const BoundingBox& obb);

// Generate a bounding sphere for a collection of spheres.
Sphere generateBoundingSphere(const std::vector<Sphere>& spheres);

#endif //GEOMETRY_UTILITY_HPP
