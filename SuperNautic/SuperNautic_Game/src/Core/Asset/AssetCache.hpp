#pragma once

#ifndef ASSET_CACHE_HPP
#define ASSET_CACHE_HPP

#include "Asset.hpp"
#include <map>

template<typename AssetT, typename KeyT>
class AssetCache
{
public:
    static Asset<AssetT> get(KeyT key);

private:
    static std::map<KeyT, std::shared_ptr<AssetT>> _cache;

    static Asset<AssetT> load(KeyT key);
};

#include "AssetCache.inl"

#endif //ASSET_CACHE_HPP
