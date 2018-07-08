#pragma once

#include <ecs/Entity.hpp>

struct MenuItem
{
    ecs::Entity parent;
    ecs::Entity previous;
    ecs::Entity next;
    ecs::Entity child;
};
