#pragma once

#ifndef APPLICATION_CONTEXT_HPP
#define APPLICATION_CONTEXT_HPP

#include "AssetCache.hpp"

struct ApplicationContext
{
    AssetCache<float, std::string> floatCache; // Just an example.
};


#endif //APPLICATION_CONTEXT_HPP
