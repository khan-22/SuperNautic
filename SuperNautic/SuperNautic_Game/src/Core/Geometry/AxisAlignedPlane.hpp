#pragma once

#ifndef AXIS_ALIGNED_PLANE_HPP
#define AXIS_ALIGNED_PLANE_HPP

struct AxisAlignedPlane
{
    enum class Axis : unsigned char
    {
        X,
        Y,
        Z,
    };
    // axis is the normal of the plane.
    // distance is distance from origo.
    AxisAlignedPlane(Axis axis, float distance)
    : axis(axis)
    , distance(distance)
    {
    }

    AxisAlignedPlane()
    {
    }

    Axis axis = Axis::X;
    float distance = 0.f;
};

#endif // AXIS_ALIGNED_PLANE_HPP
