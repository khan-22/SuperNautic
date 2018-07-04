#pragma once

#include <ecs/ecs.hpp>
#include "TypeId.hpp"
#include "EcsHelper.hpp"

namespace ecs
{

using namespace impl;

template<typename... T>
std::vector<Entity> get_entities_with() 
{ 
    return EcsHelper::get_entities_with({TypeId::index<T>...}); 
}

template<typename ComponentT, typename... WithT>
std::vector<ComponentT*> get_components_with()
{
    std::vector<void*> result = EcsHelper::get_components_with(TypeId::index<ComponentT>, {TypeId::index<WithT>...});

    std::vector<ComponentT*> components;
    for(void* c : result)
    {
        components.push_back((ComponentT*)c);
    }

    return components;
}

}