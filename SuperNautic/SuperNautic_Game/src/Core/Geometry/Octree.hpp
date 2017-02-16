#pragma once

#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "glm/glm.hpp"
#include <list>
#include <vector>

#include "Core/Geometry/CollisionMesh.hpp"

template<typename ElementT>
class Octree
{
public:
    Octree(const glm::vec3& center, float size);

    void insert(const CollisionMesh& mesh, const ElementT& element);
    bool bInsertIfNoCollision(const CollisionMesh& mesh, const ElementT& element);

private:
    static constexpr unsigned int _MAX_ELEMENTS = 5;
    std::list<std::pair<CollisionMesh, ElementT>> _elements;
    std::vector<Octree> _children;


    CollisionMesh _xPlane;
    CollisionMesh _yPlane;
    CollisionMesh _zPlane;

    void split();
    bool bHasChildren() const;
    unsigned int getSize() const;
    const std::vector<Octree*>& getIntersectingChildren(const CollisionMesh& mesh) const;
};

#include "Core/Geometry/Octree.inl"



#endif //OCTREE_HPP
