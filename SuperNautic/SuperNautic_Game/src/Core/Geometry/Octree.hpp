#pragma once

#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <set>
#include <functional>

#include "Core/Geometry/CollisionMesh.hpp"

template<typename ElementT>
class Octree
{
public:
    // Place cubic octree with its center at center
    // and size as its halflength.
    Octree(const glm::vec3& center, float size, float minNodeSize = 0.5f, unsigned short numElementsPerNode = 5);

    // Insert element along with a collision mesh.
    // Return true if element is inserted. (insertion fails if mesh does not intersect octree).
    bool bInsert(const CollisionMesh& mesh, const ElementT& element);

    // Insert element along with a collision mesh if
    // the mesh does not collide with any existing elements.
    // Return true if element is inserted.
    bool bInsertIfNoCollision(const CollisionMesh& mesh, const ElementT& element);

    // Insert element along with a collision mesh if
    // predicate returns true. The predicate's parameter
    // contains all elements that collide with the given
    // collision mesh.
    // Returns true if element is inserted.
    bool bInsertIf(const CollisionMesh& mesh, const ElementT& element, const std::function<bool(const std::vector<ElementT*>&)>& predicate);

    // Get all elements that collide with a collision mesh.
    std::vector<ElementT*> getCollisions(const CollisionMesh& mesh) const;

	bool bTestCollision(const CollisionMesh& mesh) const
	{
		return _bounds.testCollision(mesh) == CollisionMesh::CollisionResult::COLLISION;
	}

	// Hobo erase all elements that compare equal to argument... Does not merge empty nodes. /shrug
	void erase(const ElementT& element);

	// Return center
	glm::vec3 getCenter() const;

	// Return size
	float getSize() const;

private:
    class Node
    {
    public:
        using ElementPtr = std::shared_ptr<std::pair<CollisionMesh, ElementT>>;

        Node(const glm::vec3& center, float size, float minSize, unsigned short numMaxElements);

        void insert(ElementPtr element);
        void erase(const ElementT& element);
        void getIntersectingLeafNodes(const CollisionMesh& mesh, std::vector<Node*>& leafNodes) const;
        const std::vector<ElementPtr>& getElements() const;

		glm::vec3 getCenter() const;
		float getBoundingSize() const;

    private:
        static constexpr unsigned int                   _NUM_CHILDREN = 8;

        glm::vec3                                       _center;
        float                                           _size;
        CollisionMesh                                   _xPlane;
        CollisionMesh                                   _yPlane;
        CollisionMesh                                   _zPlane;
        std::vector<ElementPtr>                         _elements;
        std::vector<Node>                               _children;
        float                                           _minSize;
        unsigned short                                  _numMaxElements;

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
