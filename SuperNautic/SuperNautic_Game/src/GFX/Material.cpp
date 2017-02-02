#include "Material.hpp"


using namespace GFX;

Material::Material(const std::vector<Group>& groups)
: _groups(groups)
{

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
