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

private:
    static std::map<KeyT, std::shared_ptr<AssetT>> _cache;

    static Asset<AssetT> load(KeyT key);
};

#include "Core/Asset/AssetCache.inl"

#endif //ASSET_CACHE_HPP
