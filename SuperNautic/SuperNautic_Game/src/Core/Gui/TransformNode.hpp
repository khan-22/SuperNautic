#pragma once

#include <vector>


struct Transform;
struct WorldTransform;



struct TransformNode
{
    TransformNode* parent = nullptr;
    Transform* local = nullptr;
    WorldTransform* world = nullptr;
    std::vector<TransformNode*> children;
};