#pragma once

#include <ecs/Entity.hpp>

namespace ecs
{
namespace impl
{
    class EcsHelper
    {
        public:
            static Entity create_entity();
            static void destroy_entity(Entity& entity);

            
            static std::vector<Entity> get_entities_with(const std::vector<TypeIndex>& ids);
            static std::vector<Entity> get_entities_like(const Entity& entity);
            static std::vector<void*> get_components_with(TypeIndex type, std::vector<TypeIndex> with_types);

        private:
            static std::vector<Entity> from_ids(const std::vector<EntityId>& ids);
    };
}
}