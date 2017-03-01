#pragma once

#ifndef ASSET_CACHE_HPP
#define ASSET_CACHE_HPP

#include <map>
#include "Core/Asset/Asset.hpp"

template<typename AssetT, typename KeyT>
class AssetCache
{
public:
    static Asset<AssetT> get(KeyT key);
    static void reload();

private:
    static std::map<KeyT, std::pair<std::shared_ptr<bool>, std::shared_ptr<AssetT>>> _cache;

    static std::shared_ptr<AssetT> loadAsset(KeyT key);
    static Asset<AssetT> loadNew(KeyT key);
};

#include "Core/Asset/AssetCache.inl"

#endif //ASSET_CACHE_HPP
