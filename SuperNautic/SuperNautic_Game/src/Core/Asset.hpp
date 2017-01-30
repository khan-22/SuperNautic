#pragma once

#ifndef ASSET_HPP
#define ASSET_HPP

#include <memory>

template<typename AssetT>
class Asset
{
public:
    Asset();
    Asset(std::shared_ptr<AssetT>& asset);
    ~Asset();

    AssetT* get() const;

private:
    std::shared_ptr<AssetT> _asset;
    std::shared_ptr<AssetT>* _parentAsset;
};

#include "Asset.inl"

#endif //ASSET_HPP
