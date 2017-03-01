#include "Core/Utility/Utilities.hpp"



void createFileIfNotExists(const std::string& path)
{
    std::ifstream file(path);

    if(!file.good())
    {
        std::ofstream out(path);
    }
}
