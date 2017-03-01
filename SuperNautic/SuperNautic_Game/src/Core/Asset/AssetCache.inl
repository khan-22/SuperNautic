#include "Core/Asset/LoadAsset.hpp"

template<typename AssetT, typename KeyT>
std::map<KeyT, std::pair<std::shared_ptr<bool>, std::shared_ptr<AssetT>>> AssetCache<AssetT, KeyT>::_cache;


template<typename AssetT, typename KeyT>
std::shared_ptr<AssetT> AssetCache<AssetT, KeyT>::loadAsset(KeyT key)
{
    std::shared_ptr<AssetT> asset = ::loadAsset<AssetT>(key);
    if(asset == nullptr)
    {
        LOG_ERROR("Asset Cache failed to load asset of type \"", typeid(AssetT).name(), "\": ", key);
        //throw std::runtime_error("");
    }
    return asset;
}

template<typename AssetT, typename KeyT>
Asset<AssetT> AssetCache<AssetT, KeyT>::loadNew(KeyT key)
{
    auto& pair = _cache[key];
    pair.first.reset(new bool());
    pair.second = loadAsset(key);
    return Asset<AssetT>(pair.first, pair.second);
}

template<typename AssetT, typename KeyT>
Asset<AssetT> AssetCache<AssetT, KeyT>::get(KeyT key)
{
    auto cachedIterator = _cache.find(key);
    if(cachedIterator == _cache.end() || cachedIterator->second.first.use_count() == 1)
    {
        return loadNew(key);
    }

    return Asset<AssetT>(cachedIterator->second.first, cachedIterator->second.second);
}

template<typename AssetT, typename KeyT>
void AssetCache<AssetT, KeyT>::reload()
{
    for(auto& pair : _cache)
    {
        if(pair.second.first.use_count() != 1)
        {
            pair.second.second = loadAsset(pair.first);
        }
    }
}
