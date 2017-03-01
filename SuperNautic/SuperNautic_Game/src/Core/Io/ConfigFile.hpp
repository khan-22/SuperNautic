#pragma once

#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

#include <string>
#include <map>
#include <sstream>

#include "Core/Io/Log.hpp"

class ConfigFile
{
public:

    // Create empty, in-memory config
    ConfigFile();

    // Load config from file to memory
    ConfigFile(const std::string& filePath);

    // Write this config to file
    // Returns true if write was successful
    bool write(const std::string& filePath) const;

    // Load config from file to memory
    // Returns true if read was successful
    bool read(const std::string& filePath);

    // Set element value paired with key.
    template<typename T>
    void set(const std::string& key, const T& value)
    {
        std::stringstream sstream;
        sstream << value;
        _elements[key] = sstream.str();
    }

    // Get element value paired with key as type T
    template<typename T>
    T get(const std::string& key)
    {
        std::stringstream sstream(_elements[key]);
        T value;
        sstream >> value;
        LOG(value);

        return value;
    }






    std::map<std::string, std::string> _elements;
};

template<>
std::string ConfigFile::get<std::string>(const std::string& key);


#endif // CONFIG_FILE_HPP
