#include "Core/Io/ConfigFile.hpp"
#include "Core/Io/Log.hpp"



ConfigFile::ConfigFile()
{

}

// Load config from file to memory
ConfigFile::ConfigFile(const std::string& filePath)
{
    if(!read(filePath))
    {
        LOG_ERROR("Failed to read config file at \"", filePath, "\".");
    }
}

// Write this config to file
bool ConfigFile::write(const std::string& filePath) const
{
    std::ofstream file(filePath);

    if(!file.good())
    {
        return false;
    }

    for(const auto& pair : _elements)
    {
        file << pair.first << " = " << pair.second << std::endl;
    }

    file.close();

    return true;
}

// Load config from file to memory
bool ConfigFile::read(const std::string& filePath)
{
    _elements.clear();
    std::ifstream file(filePath);
    if(!file.good())
    {
        return false;
    }

    do
    {
        std::string key;
        char equalSign;
        std::string value;

        file >> key;
        file >> std::ws;

        if(!file.good())
        {
            return false;
        }

        file.read(&equalSign, 1);
        file >> std::ws;

        if(equalSign != '=' || !file.good())
        {
            return false;
        }

        std::getline(file, value);
        file >> std::ws;

        _elements[key] = value;

    } while(file.good());

    file.close();
    return true;
}

template<>
bool ConfigFile::get<std::string>(const std::string& key, std::string& out)
{
    auto it = _elements.find(key);
    if(it == _elements.end())
    {
        return false;
    }

    out = it->second;

    return true;
}
