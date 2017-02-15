#pragma once

#ifndef AXIS_ALIGNED_PLANE_HPP
#define AXIS_ALIGNED_PLANE_HPP

struct AxisAlignedPlane
{
    enum class Axis : unsigned char
    {
        X = 0,
        Y = 1,
        Z = 2,
    };
    // axis is the normal of the plane.
    // distance is distance from origo.
    AxisAlignedPlane(Axis axis, float distance);
    AxisAlignedPlane();

    unsigned char getAxisIndex() const;

    Axis axis = Axis::X;
    float distance = 0.f;
};

#endif // AXIS_ALIGNED_PLANE_HPP
