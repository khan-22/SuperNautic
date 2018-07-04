#include <ecs/ecs.hpp>
#include "DefaultContext.hpp"

namespace ecs
{

namespace impl
{

}

void initialize()
{
    static bool is_initialized = false;

    if (!is_initialized)
    {
        DefaultContext::Manager = EntityManager();
        is_initialized = true;
    }
}

Entity create_entity()
{
    return EcsHelper::create_entity();
}

void destroy_entity(Entity& entity)
{
    EcsHelper::destroy_entity(entity);
}

std::vector<Entity> get_entities_like(const Entity& entity)
{
    return EcsHelper::get_entities_like(entity);
}


}