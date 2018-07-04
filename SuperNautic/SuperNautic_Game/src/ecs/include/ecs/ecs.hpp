#pragma once

#include "Entity.hpp"

namespace ecs
{
    void initialize();

    Entity create_entity();
    void destroy_entity();

    template<typename... T>
    std::vector<Entity> get_entities_with();
    
    std::vector<Entity> get_entities_like(const Entity& entity);

    template<typename ComponentT, typename... WithT>
    std::vector<ComponentT*> get_components_with(); 
}

#include "../../impl/ecs.inl"