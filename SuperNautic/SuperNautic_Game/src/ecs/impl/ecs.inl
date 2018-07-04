#pragma once

#include <ecs/ecs.hpp>
#include "TypeId.hpp"
#include "EcsHelper.hpp"

namespace ecs
{

using namespace impl;


template<typename... T>
Entity get_entity_with()
{
    return EcsHelper::get_entity_with({TypeId::index<T>...});
}

template<typename... T>
std::vector<Entity> get_entities_with() 
{ 
    return EcsHelper::get_entities_with({TypeId::index<T>...}); 
}

template<typename ComponentT, typename... WithT>
ComponentT* get_component_with()
{
    return (ComponentT*)EcsHelper::get_component_with(TypeId::index<ComponentT>, {TypeId::index<WithT>...});
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