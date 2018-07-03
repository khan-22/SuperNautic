#pragma once

#include <ecs/ecs.hpp>
#include "DefaultContext.hpp"
#include "TypeId.hpp"

namespace ecs
{

template<typename ComponentT, typename... WithT>
std::vector<ComponentT*> get_components_with()
{
    std::vector<void*> result = impl::DefaultManager.get_components_with(TypeId::index<ComponentT>, {TypeId::index<WithT>...});

    std::vector<ComponentT*> components;
    for(void* c : result)
    {
        components.push_back((ComponentT*)c);
    }

    return components;
}

}