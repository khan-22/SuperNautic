#pragma once

#include <ecs/ecs.hpp>
#include "TypeId.hpp"

namespace ecs
{
namespace impl
{
    std::vector<void*> get_components_with(TypeIndex type, std::vector<TypeIndex> with_types);
}

template<typename ComponentT, typename... WithT>
std::vector<ComponentT*> get_components_with()
{
    std::vector<void*> result = impl::get_components_with(TypeId::index<ComponentT>, {TypeId::index<WithT>...});

    std::vector<ComponentT*> components;
    for(void* c : result)
    {
        components.push_back((ComponentT*)c);
    }

    return components;
}

}