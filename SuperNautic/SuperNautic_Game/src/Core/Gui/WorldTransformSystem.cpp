#include "WorldTransformSystem.hpp"

#include <cassert>

#include "TransformNode.hpp"
#include "Transform.hpp"
#include "WorldTransform.hpp"

#include <ecs/ecs.hpp>


void WorldTransformSystem::update_node(TransformNode& node)
{
    assert(node.world);
    for(TransformNode* child : node.children)
    {
        assert(child->world);
        child->world->sf::Transform::operator=(*node.world * *child->world);
    }

    for(TransformNode* child : node.children)
    {
        update_node(*child);
    }
}

void WorldTransformSystem::update()
{
    for(ecs::Entity e : ecs::get_entities_with<Transform, WorldTransform>())
    {
        WorldTransform* world = e;
        Transform* local = e;
        world->sf::Transform::operator=(*local);
    }


    for(TransformNode* node : ecs::get_components_with<TransformNode>())
    {
        if(!node->parent)
        {
            update_node(*node);
        }
    }
}
