#include <ecs/ecs.hpp>
#include "DefaultContext.hpp"

namespace ecs
{

Entity create_entity()
{
    return Entity::create();
}

void destroy_entity(Entity& entity)
{
    entity.destroy();
}


}