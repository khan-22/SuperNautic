#include "GFX/Resources/Loaders/MaterialLoader.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"


using namespace GFX;


std::shared_ptr<Material> MaterialLoader::load(const std::string& filePath)
{
    std::ifstream file(filePath);
    if(!file.good())
    {
        LOG_ERROR("Failed to read material file at \"", filePath, "\". File not found.");
        return nullptr;
    }

    std::vector<TextureAsset> diffuses;
    std::vector<TextureAsset> speculars;
    std::vector<TextureAsset> normals;
    while(file.good())
    {
        std::string type;
        file >> type;

        std::vector<TextureAsset>* typeContainer = nullptr;
        if(type == "DIFFUSE")
        {
            typeContainer = &diffuses;
        }
        else if(type == "SPECULAR")
        {
            typeContainer = &speculars;
        }
        else if(type == "NORMAL")
        {
            typeContainer = &normals;
        }
        else
        {
            LOG_ERROR("Failed to read material file at \"", filePath, "\". Expected valid type, got \"", type, "\".");
            return nullptr;
        }

        if(!file.good())
        {
            LOG_ERROR("Failed to read material file at \"", filePath, "\". Unexpected end of file.");
            return nullptr;
        }

        unsigned int id;
        file >> id;
        if(id >= typeContainer->size())
        {
            typeContainer->resize(id + 1);
        }

        if(!file.good())
        {
            LOG_ERROR("Failed to read material file at \"", filePath, "\". Unexpected end of file.");
            return nullptr;
        }

        file >> std::ws;

        std::string path;
        std::getline(file, path);

        TextureAsset texture = TextureCache::get(path);
        if(texture.get() == nullptr)
        {
            LOG_ERROR("Failed to read material file at \"", filePath,
                      "\". Could not find specified texture path: \"", path, "\".");
            return nullptr;
        }

        typeContainer->at(id) = texture;
    }

    if(diffuses.size() != speculars.size() || speculars.size() != normals.size())
    {
        LOG_ERROR("Failed to read material file at \"", filePath, "\". Texture count cannot vary.");
        return nullptr;
    }

    for(const TextureAsset& a : diffuses)
    {
        if(a.get() == nullptr)
        {
            LOG_ERROR("Failed to read material file at \"", filePath, "\". Texture ID range must be [0...n].");
            return nullptr;
        }
    }
    for(const TextureAsset& a : speculars)
    {
        if(a.get() == nullptr)
        {
            LOG_ERROR("Failed to read material file at \"", filePath, "\". Texture ID range must be [0...n].");
            return nullptr;
        }
    }
    for(const TextureAsset& a : normals)
    {
        if(a.get() == nullptr)
        {
            LOG_ERROR("Failed to read material file at \"", filePath, "\". Texture ID range must be [0...n].");
            return nullptr;
        }
    }

    std::vector<Material::Group> groups;
    for(size_t i = 0; i < diffuses.size(); i++)
    {
        groups.emplace_back(diffuses[i], speculars[i], normals[i]);
    }

    return std::make_shared<Material>(groups);
}
