#pragma once

#ifndef ASSET_HPP
#define ASSET_HPP

#include <memory>

template<typename AssetT>
class Asset
{
public:
    // Create empty asset
    Asset();

    // Create shared asset
    Asset(std::shared_ptr<bool> refCount, std::shared_ptr<AssetT>& asset);

    // Dtor
    ~Asset();

    // Get element
    AssetT* get() const;

private:
    std::shared_ptr<AssetT>* _asset;
    std::shared_ptr<bool> _refCount;
};

#include "Core/Asset/Asset.inl"

#endif //ASSET_HPP
