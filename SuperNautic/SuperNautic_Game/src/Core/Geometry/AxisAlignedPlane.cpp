#include <cassert>

#include "Core/Geometry/AxisAlignedPlane.hpp"

AxisAlignedPlane::AxisAlignedPlane(Axis axis, float distance)
: axis(axis)
, distance(distance)
{
}

AxisAlignedPlane::AxisAlignedPlane()
{
}

unsigned char AxisAlignedPlane::getAxisIndex() const
{
    assert((unsigned char)axis <= 2);
    return (unsigned char)axis;
}
