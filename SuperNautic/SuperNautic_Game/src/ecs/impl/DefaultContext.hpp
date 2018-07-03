#pragma once

#include "../src/EntityManager.hpp"

namespace ecs
{
namespace impl
{
    class DefaultContext
    {
        public:
            static EntityManager Manager;
    };
}
}