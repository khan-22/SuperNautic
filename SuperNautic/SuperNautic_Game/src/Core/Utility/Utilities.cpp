#include "Core/Utility/Utilities.hpp"


bool bIsFloatEq(float a, float b)
{
    static constexpr float EPSILON = 0.000001f;
    return a < b + EPSILON && a > b - EPSILON;
}


std::string randString(size_t size)
{
    std::string str(size, 'A');
    for(char& c : str)
    {
        switch(rand() % 3)
        {
            case 0:
                c = '0' + rand() % ('9' - '0');
                break;
            case 1:
                c = 'A' + rand() % ('Z' - 'A');
                break;
            case 2:
                c = 'a' + rand() % ('z' - 'a');
                break;
            default:
                break;
        }
    }

    return str;
}
