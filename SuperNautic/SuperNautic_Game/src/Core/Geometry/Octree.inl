#include "Core/Io/Log.hpp"
#include "Octree.hpp"

template<typename ElementT>
Octree<ElementT>::Octree(const glm::vec3& center, float size, float minNodeSize, unsigned short numElementsPerNode)
: _bounds(BoundingBox
(
    center,
    {glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},
    {size, size, size}
))
, _root(center, size, minNodeSize, numElementsPerNode)
{

}

template<typename ElementT>
bool Octree<ElementT>::bInsert(const CollisionMesh& mesh, const ElementT& element)
{
    if(_bounds.testCollision(mesh) != CollisionMesh::CollisionResult::COLLISION)
    {
        return false;
    }

    std::vector<Node*> nodes;
    _root.getIntersectingLeafNodes(mesh, nodes);
    assert(!nodes.empty());

    typename Node::ElementPtr nodeElement(new std::pair<CollisionMesh, ElementT>(mesh, element));
    for(Node* node : nodes)
    {
        node->insert(nodeElement);
    }

    return true;
}

template<typename ElementT>
bool Octree<ElementT>::bInsertIfNoCollision(const CollisionMesh& mesh, const ElementT& element)
{
	if (_bounds.testCollision(mesh) != CollisionMesh::CollisionResult::COLLISION)
	{
		return false;
	}

    std::vector<Node*> nodes;
    _root.getIntersectingLeafNodes(mesh, nodes);
    assert(!nodes.empty());


    for(const typename Node::ElementPtr& nodeElement : getUniqueElements(nodes))
    {
        if(mesh.testCollision(nodeElement->first) == CollisionMesh::CollisionResult::COLLISION)
        {
            return false;
        }
    }

    typename Node::ElementPtr nodeElement(new std::pair<CollisionMesh, ElementT>(mesh, element));
    for(Node* node : nodes)
    {
        node->insert(nodeElement);
    }

    return true;
}


template<typename ElementT>
bool Octree<ElementT>::bInsertIf(const CollisionMesh& mesh, const ElementT& element
	, const std::function<bool(const std::vector<ElementT*>&)>& predicate)
{
	if (_bounds.testCollision(mesh) != CollisionMesh::CollisionResult::COLLISION)
	{
		return false;
	}

    std::vector<Node*> nodes;
    _root.getIntersectingLeafNodes(mesh, nodes);
    assert(!nodes.empty());

    std::vector<ElementT*> collisions;
    for(const typename Node::ElementPtr& nodeElement : getUniqueElements(nodes))
    {
        if(mesh.testCollision(nodeElement->first) == CollisionMesh::CollisionResult::COLLISION)
        {
            collisions.push_back(&nodeElement->second);
        }
    }

    if(!predicate(collisions))
    {
        return false;
    }

    typename Node::ElementPtr nodeElement(new std::pair<CollisionMesh, ElementT>(mesh, element));
    for(Node* node : nodes)
    {
        node->insert(nodeElement);
    }

    return true;
}

template<typename ElementT>
std::vector<ElementT*> Octree<ElementT>::getCollisions(const CollisionMesh& mesh) const
{
    std::vector<Node*> nodes;
    _root.getIntersectingLeafNodes(mesh, nodes);
    assert(!nodes.empty());

    std::vector<ElementT*> collisions;
    for(const typename Node::ElementPtr& nodeElement : getUniqueElements(nodes))
    {
        if(mesh.testCollision(nodeElement->first) == CollisionMesh::CollisionResult::COLLISION)
        {
            collisions.push_back(&nodeElement->second);
        }
    }
    return collisions;
}


template<typename ElementT>
std::set<typename Octree<ElementT>::Node::ElementPtr> Octree<ElementT>::getUniqueElements(const std::vector<Node*>& nodes)
{
    std::set<typename Node::ElementPtr> elements;
    for(Node* node : nodes)
    {
        const auto& nodeElements = node->getElements();
        elements.insert(nodeElements.begin(), nodeElements.end());
    }
    return elements;
}

template<typename ElementT>
void Octree<ElementT>::erase(const ElementT& element)
{
    _root.erase(element);
}

template<typename ElementT>
inline glm::vec3 Octree<ElementT>::getCenter() const
{
	return _root.getCenter();
}

template<typename ElementT>
inline float Octree<ElementT>::getSize() const
{
	return _root.getBoundingSize();
}


template<typename ElementT>
Octree<ElementT>::Node::Node(const glm::vec3& center, float size, float minNodeSize, unsigned short numMaxElements)
: _center(center)
, _size(size)
, _xPlane(AxisAlignedPlane(AxisAlignedPlane::Axis::X, center.x))
, _yPlane(AxisAlignedPlane(AxisAlignedPlane::Axis::Y, center.y))
, _zPlane(AxisAlignedPlane(AxisAlignedPlane::Axis::Z, center.z))
, _minSize(minNodeSize)
, _numMaxElements(numMaxElements)
{

}

template<typename ElementT>
void Octree<ElementT>::Node::insert(ElementPtr element)
{
    if(!bIsLeafNode())
    {
        for(Node& child : _children)
        {
            child.insert(element);
        }
        return;
    }

    _elements.emplace_back(element);
    if(getSize() >= _numMaxElements && _size > _minSize)
    {
        split();
    }
}

template<typename ElementT>
void Octree<ElementT>::Node::getIntersectingLeafNodes(const CollisionMesh& mesh, std::vector<Node*>& leafNodes) const
{
    if(bIsLeafNode())
    {
        leafNodes.push_back((Node*)this);
        return;
    }

    CollisionMesh::CollisionResult collisionResults[3] =
    {
        mesh.testCollision(_xPlane),
        mesh.testCollision(_yPlane),
        mesh.testCollision(_zPlane)
    };

    static constexpr unsigned char FRONT_MASKS[3] =
    {
        0b11110000,
        0b11001100,
        0b10101010
    };
    static_assert(sizeof(FRONT_MASKS[0])* 8 == _NUM_CHILDREN, "Mask bit size must match octree split size.");

    unsigned char intersectingNodeIndices = 0b11111111;
    assert(sizeof(intersectingNodeIndices) * 8 == _NUM_CHILDREN);
    for(unsigned char i = 0; i < 3; i++)
    {
        switch(collisionResults[i])
        {
            case CollisionMesh::CollisionResult::BACK:
                intersectingNodeIndices &= ~FRONT_MASKS[i];
                break;
            case CollisionMesh::CollisionResult::FRONT:
                intersectingNodeIndices &= FRONT_MASKS[i];
                break;
            case CollisionMesh::CollisionResult::COLLISION:
                break;
            default:
                LOG_ERROR("Unexpected collision result: ", (int)collisionResults[i]);
                return;
        }
    }

    for(unsigned char i = 0; i < sizeof(intersectingNodeIndices) * 8; i++)
    {
        if(intersectingNodeIndices & (1 << i))
        {
            _children[i].getIntersectingLeafNodes(mesh, leafNodes);
        }
    }
}

template<typename ElementT>
const std::vector<typename Octree<ElementT>::Node::ElementPtr>& Octree<ElementT>::Node::getElements() const
{
    return _elements;
}


template<typename ElementT>
bool Octree<ElementT>::Node::bIsCollisionResultValid(CollisionMesh::CollisionResult result) const
{
    switch(result)
    {
        case CollisionMesh::CollisionResult::BACK:
        case CollisionMesh::CollisionResult::FRONT:
        case CollisionMesh::CollisionResult::COLLISION:
            return true;

        default:
            LOG_ERROR("Unexpected collision result: ", (int)result);
            return false;
    }
}

template<typename ElementT>
void Octree<ElementT>::Node::split()
{
    static constexpr glm::vec3 CHILD_OFFSETS[_NUM_CHILDREN] =
    {
        glm::vec3(-1.f, -1.f, -1.f),// - - -
        glm::vec3(-1.f, -1.f, 1.f), // - - +
        glm::vec3(-1.f, 1.f, -1.f), // - + -
        glm::vec3(-1.f, 1.f, 1.f),  // - + +
        glm::vec3(1.f, -1.f, -1.f), // + - -
        glm::vec3(1.f, -1.f, 1.f),  // + - +
        glm::vec3(1.f, 1.f, -1.f),  // + + -
        glm::vec3(1.f, 1.f, 1.f)    // + + +
    };

    float childSize = _size / 2.f;
    _children.reserve(_NUM_CHILDREN);
    for(unsigned int i = 0; i < _NUM_CHILDREN; i++)
    {
        _children.emplace_back(_center + CHILD_OFFSETS[i] * childSize, childSize, _minSize, _numMaxElements);
    }

    for(const ElementPtr& element : _elements)
    {
        std::vector<Node*> nodes;
        getIntersectingLeafNodes(element->first, nodes);
        assert(!nodes.empty());
        for(Node* node : nodes)
        {
            node->insert(element);
        }
    }

    _elements.clear();
}

template<typename ElementT>
bool Octree<ElementT>::Node::bIsLeafNode() const
{
    return _children.empty();
}

template<typename ElementT>
inline glm::vec3 Octree<ElementT>::Node::getCenter() const
{
	return _center;
}

template<typename ElementT>
inline float Octree<ElementT>::Node::getBoundingSize() const
{
	return _size;
}

template<typename ElementT>
unsigned int Octree<ElementT>::Node::getSize() const
{
    return _elements.size();
}

template<typename ElementT>
void Octree<ElementT>::Node::erase(const ElementT& element)
{
    auto end = std::remove_if(_elements.begin(), _elements.end(), [&element](const ElementPtr& e)
    {
        return element == e->second;
    });

    _elements.erase(end, _elements.end());

    for(Node& child : _children)
    {
        child.erase(element);
    }
}
