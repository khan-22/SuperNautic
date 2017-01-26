#include "LoadAsset.hpp"

template<typename AssetT, typename KeyT>
Asset<AssetT> AssetCache<AssetT, KeyT>::load(KeyT key)
{
    std::shared_ptr<AssetT> asset = loadAsset<AssetT>(key);
    if(asset == nullptr)
    {
        LOG_ERROR("Failed to load asset.");
        throw std::runtime_error("");
    }

    std::shared_ptr<AssetT>& cachedAsset = _cache[key];
    cachedAsset = asset;
    return Asset<AssetT>(cachedAsset);
}

template<typename AssetT, typename KeyT>
Asset<AssetT> AssetCache<AssetT, KeyT>::get(KeyT key)
{
    auto cachedIterator = _cache.find(key);
    if(cachedIterator == _cache.end())
    {
        return load(key);
    }

    return Asset<AssetT>(cachedIterator->second);
}