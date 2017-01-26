
#include <cassert>


template<typename AssetT>
Asset<AssetT>::Asset(std::shared_ptr<AssetT>& asset)
: _asset(asset)
, _parentAsset(asset)
{
    assert(asset != nullptr);
}

template<typename AssetT>
Asset<AssetT>::~Asset()
{
    if(_asset.use_count() == 2)
    {
        _parentAsset.reset();
    }
}

template<typename AssetT>
AssetT* Asset<AssetT>::get() const
{
    return _asset.get();
}
