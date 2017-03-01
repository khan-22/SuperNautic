
#include "Core/Io/Log.hpp"

#include <cassert>

template<typename AssetT>
Asset<AssetT>::Asset()
: _asset(nullptr)
, _refCount(new bool())
{

}

template<typename AssetT>
Asset<AssetT>::Asset(std::shared_ptr<bool> refCount, std::shared_ptr<AssetT>& asset)
: _asset(&asset)
, _refCount(refCount)
{
    //assert(asset != nullptr);
}

template<typename AssetT>
Asset<AssetT>::~Asset()
{
    if(_asset == nullptr)
    {
        return;
    }

    if(_refCount.use_count() == 2)
    {
        _asset->reset();
    }
}

template<typename AssetT>
AssetT* Asset<AssetT>::get() const
{
    if(_asset != nullptr)
    {
        return _asset->get();
    }
    else
    {
        return nullptr;
    }
}
