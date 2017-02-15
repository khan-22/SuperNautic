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
    // The normal of the plane points towards positive axis.
    AxisAlignedPlane(Axis axis, float distanceFromAxis)
    : axis(axis)
    , distance(distanceFromAxis)
    {
    }

    AxisAlignedPlane()
    {
    }

    Axis axis = Axis::X;
    float distance = 0.f;
};

#endif // AXIS_ALIGNED_PLANE_HPP
