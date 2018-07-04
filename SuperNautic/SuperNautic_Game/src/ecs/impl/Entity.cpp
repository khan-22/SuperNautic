#include <ecs/Entity.hpp>
#include "../src/EntityManager.hpp"
#include "DefaultContext.hpp"
#include <type_traits>

namespace ecs
{

using namespace impl;

Entity::Entity()
: _id(0)
{
}

Entity::Entity(EntityId id)
: _id(id)
{
}

void* Entity::get(TypeIndex id) const
{
    return DefaultContext::Manager.get_component(id, _id);
}

void* Entity::attach(TypeIndex id)
{
    return DefaultContext::Manager.attach(id, _id);
}

bool Entity::detach(TypeIndex id, void* data)
{
    return DefaultContext::Manager.detach(id, data, _id);
}

std::vector<void*> Entity::detach(TypeIndex id)
{
    return DefaultContext::Manager.detach(id, _id);
}


}