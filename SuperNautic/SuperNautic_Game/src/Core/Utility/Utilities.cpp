#include "Core/Utility/Utilities.hpp"


bool floatEq(float a, float b)
{
    static constexpr float EPSILON = 0.000001f;
    return a < b + EPSILON && a > b - EPSILON;
}
