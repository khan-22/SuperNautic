#pragma once

#include "Entity.hpp"

namespace ecs
{
    Entity create_entity();
    void destroy_entity();

    template<typename ComponentT, typename... WithT>
    std::vector<ComponentT*> get_components_with(); 
}

#include "../../impl/ecs.inl"