#include "Material.hpp"


using namespace GFX;

Material::Material(const std::vector<Group>& groups)
: _groups(groups)
{

}

void Material::bind(unsigned int groupId) const
{
    if(groupId >= _groups.size())
    {
        throw std::runtime_error("Group ID out of range.");
    }

    const Group& group = _groups[groupId];
    group.normal.get()->bind(2);
    group.specular.get()->bind(1);
    group.diffuse.get()->bind(0);
}

void Material::unbind(unsigned int groupId) const
{
    if(groupId >= _groups.size())
    {
        throw std::runtime_error("Group ID out of range.");
    }

    const Group& group = _groups[groupId];
    group.normal.get()->unbind(2);
    group.specular.get()->unbind(1);
    group.diffuse.get()->unbind(0);
}

size_t Material::getNumGroups() const
{
    return _groups.size();
}


/////////////////////////
// Material::Group
/////////////////////////
Material::Group::Group(TextureAsset& diffuse, TextureAsset& specular, TextureAsset& normal)
: diffuse(diffuse)
, specular(specular)
, normal(normal)
{
}
