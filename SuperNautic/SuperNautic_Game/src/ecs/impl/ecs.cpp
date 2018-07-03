#include <ecs/ecs.hpp>
#include "DefaultContext.hpp"

namespace ecs
{

namespace impl
{

std::vector<void*> get_components_with(TypeIndex type, std::vector<TypeIndex> with_types)
{
    return DefaultContext::Manager.get_components_with(type, with_types);
}

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
    return Entity::create();
}

void destroy_entity(Entity& entity)
{
    entity.destroy();
}



}