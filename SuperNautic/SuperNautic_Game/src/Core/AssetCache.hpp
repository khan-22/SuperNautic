#pragma once

#ifndef ASSET_CACHE_HPP
#define ASSET_CACHE_HPP

#include <map>

#include "Asset.hpp"

template<typename AssetT, typename KeyT>
class AssetCache
{
public:
    Asset<AssetT> get(KeyT key);

private:
    std::map<KeyT, std::shared_ptr<AssetT>> _cache;

    Asset<AssetT> load(KeyT key);
};

#include "AssetCache.inl"

#endif //ASSET_CACHE_HPP
