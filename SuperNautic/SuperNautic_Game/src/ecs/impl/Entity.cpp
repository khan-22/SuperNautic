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

bool Entity::is_null() const
{
    return _id == 0;
}

bool Entity::operator==(std::nullptr_t null_ptr) const
{
    return is_null();
}

bool Entity::operator!() const
{
    return is_null();
}

Entity::operator bool() const
{
    return !is_null();
}

Entity& Entity::operator=(std::nullptr_t null_ptr)
{
    _id = 0;
    return *this;
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