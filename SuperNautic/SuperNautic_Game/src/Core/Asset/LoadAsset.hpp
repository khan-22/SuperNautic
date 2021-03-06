#pragma once

#ifndef LOAD_ASSET_HPP
#define LOAD_ASSET_HPP

#include <memory>

#include "Core/Io/Log.hpp"

template<typename AssetT, typename KeyT>
std::shared_ptr<AssetT> loadAsset(KeyT key);

#endif //LOAD_ASSET_HPP
