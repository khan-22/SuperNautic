#pragma once

#ifndef GEOMETRY_UTILITY_HPP
#define GEOMETRY_UTILITY_HPP

#include "Core/Geometry/Sphere.hpp"
#include "Core/Geometry/BoundingBox.hpp"

Sphere generateBoundingSphere(const BoundingBox& obb);

#endif //GEOMETRY_UTILITY_HPP
