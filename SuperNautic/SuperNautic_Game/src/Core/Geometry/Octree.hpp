#pragma once

#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include "Core/Geometry/CollisionMesh.hpp"

template<typename ElementT>
class Octree
{
public:
    // Place cubic octree with its center at center
    // and size as its halflength.
    Octree(const glm::vec3& center, float size);

    // Insert element along with a collision mesh.
    // Return true if element is inserted. (insertion fails if mesh does not intersect octree).
    bool bInsert(const CollisionMesh& mesh, const ElementT& element);

    // Insert element along with a collision mesh if
    // the mesh does not collide with any existing elements.
    // Return true if element is inserted.
    bool bInsertIfNoCollision(const CollisionMesh& mesh, const ElementT& element);

    // Get all elements that collide with a collision mesh.
    std::vector<ElementT*> getCollisions(const CollisionMesh& mesh) const;

private:
    class Node
    {
    public:
        using ElementPtr = std::shared_ptr<std::pair<CollisionMesh, ElementT>>;

        Node(const glm::vec3& center, float size);

        void insert(ElementPtr element);
        void getIntersectingLeafNodes(const CollisionMesh& mesh, std::vector<Node*>& leafNodes) const;
        const std::vector<ElementPtr>& getElements() const;

    private:
        static constexpr unsigned int                   _MAX_ELEMENTS = 5;
        static constexpr unsigned int                   _NUM_CHILDREN = 8;
        static constexpr float                          _MIN_SIZE = 0.5f;

        glm::vec3                                       _center;
        float                                           _size;
        CollisionMesh                                   _xPlane;
        CollisionMesh                                   _yPlane;
        CollisionMesh                                   _zPlane;
        std::vector<ElementPtr>                         _elements;
        std::vector<Node>                               _children;

        void split();
        bool bIsLeafNode() const;
        unsigned int getSize() const;
        bool bIsCollisionResultValid(CollisionMesh::CollisionResult result) const;

    };


    CollisionMesh   _bounds;
    Node            _root;

    static std::set<typename Node::ElementPtr> getUniqueElements(const std::vector<Node*>& nodes);
};

#include "Core/Geometry/Octree.inl"



#endif //OCTREE_HPP
