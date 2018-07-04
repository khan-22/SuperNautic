#include "EcsHelper.hpp"
#include "DefaultContext.hpp"

namespace ecs
{
namespace impl
{

    
Entity EcsHelper::create_entity()
{
    return Entity(DefaultContext::Manager.create_entity());
}

void EcsHelper::destroy_entity(Entity& entity)
{
    DefaultContext::Manager.destroy_entity(entity._id);
    entity._id = 0;    
}

std::vector<Entity> EcsHelper::get_entities_with(const std::vector<TypeIndex>& ids)
{
    return from_ids(DefaultContext::Manager.get_entities_with(ids));
}

std::vector<Entity> EcsHelper::get_entities_like(const Entity& entity)
{
    return from_ids(DefaultContext::Manager.get_entities_like(entity._id));
}


std::vector<void*> EcsHelper::get_components_with(TypeIndex type, std::vector<TypeIndex> with_types)
{
    return DefaultContext::Manager.get_components_with(type, with_types);
}

std::vector<Entity> EcsHelper::from_ids(const std::vector<EntityId>& ids)
{
    std::vector<Entity> entities;
    entities.reserve(ids.size());
    for(EntityId id : ids)
    {
        entities.push_back(id);
    }

    return entities;
}




}
}