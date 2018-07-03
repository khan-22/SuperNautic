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

Entity Entity::create()
{
    return Entity(DefaultManager.create_entity());
}

void Entity::destroy(Entity& entity)
{
    entity.destroy();
}

void Entity::destroy()
{
    DefaultManager.destroy_entity(_id);
    _id = 0;
}


std::vector<Entity> Entity::get_with(const std::vector<TypeIndex>& ids)
{
    return from_ids(DefaultManager.get_entities_with(ids));
}

std::vector<Entity> Entity::get_like(const Entity& entity)
{
    return from_ids(DefaultManager.get_entities_like(entity._id));
}

std::vector<Entity> Entity::get_like() const
{
    return from_ids(DefaultManager.get_entities_like(_id));
}

std::vector<Entity> Entity::from_ids(const std::vector<EntityId>& ids)
{
    std::vector<Entity> entities;
    entities.reserve(ids.size());
    for(EntityId id : ids)
    {
        entities.push_back(id);
    }

    return entities;
}


void* Entity::get(TypeIndex id) const
{
    return DefaultManager.get_component(id, _id);
}

void* Entity::attach(TypeIndex id)
{
    return DefaultManager.attach(id, _id);
}

bool Entity::detach(TypeIndex id, void* data)
{
    return DefaultManager.detach(id, data, _id);
}

std::vector<void*> Entity::detach(TypeIndex id)
{
    return DefaultManager.detach(id, _id);
}


}