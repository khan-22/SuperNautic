#pragma once

struct TransformNode;

class WorldTransformSystem
{
    public:
        static void update();

    private:
        static void update_node(TransformNode& node);
};
