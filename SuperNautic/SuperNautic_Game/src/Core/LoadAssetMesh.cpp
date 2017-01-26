// EXAMPLE THING.
// TODO: Talk Karl/Toby through about this.

#include "LoadAsset.hpp"

struct Mesh
{

};

template<>
std::shared_ptr<Mesh> loadAsset<Mesh>(std::string key)
{
    return std::shared_ptr<Mesh>(new Mesh());
}
