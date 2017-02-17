#include "Core/Io/Log.hpp"

template<typename ElementT>
Octree<ElementT>::Octree(const glm::vec3& center, float size)
: _bounds(BoundingBox
(
    center,
    {glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)},
    {size, size, size}
))
, _root(center, size)
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
    std::vector<Node*> nodes;
    _root.getIntersectingLeafNodes(mesh, nodes);
    assert(!nodes.empty());

    for(Node* node : nodes)
    {
        for(const typename Node::ElementPtr& nodeElement : node->getElements())
        {
            if(mesh.testCollision(nodeElement->first) == CollisionMesh::CollisionResult::COLLISION)
            {
                return false;
            }
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
Octree<ElementT>::Node::Node(const glm::vec3& center, float size)
: _center(center)
, _size(size)
, _xPlane(AxisAlignedPlane(AxisAlignedPlane::Axis::X, center.x))
, _yPlane(AxisAlignedPlane(AxisAlignedPlane::Axis::Y, center.y))
, _zPlane(AxisAlignedPlane(AxisAlignedPlane::Axis::Z, center.z))
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
    if(getSize() >= _MAX_ELEMENTS && _size > _MIN_SIZE)
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

    using R = CollisionMesh::CollisionResult;
    R resultX = mesh.testCollision(_xPlane);
    R resultY = mesh.testCollision(_yPlane);
    R resultZ = mesh.testCollision(_zPlane);

    unsigned char intersectingNodeIndices = 0b11111111;
    assert(sizeof(intersectingNodeIndices) * 8 == _NUM_CHILDREN);
    switch(resultX)
    {
        case CollisionMesh::CollisionResult::BACK:
            intersectingNodeIndices &= 0b00001111;
            break;
        case CollisionMesh::CollisionResult::FRONT:
            intersectingNodeIndices &= 0b11110000;
            break;
        case CollisionMesh::CollisionResult::COLLISION:
            break;
        default:
            LOG_ERROR("Unexpected collision result: ", (int)resultX);
            return;
    }

    switch(resultY)
    {
        case CollisionMesh::CollisionResult::BACK:
            intersectingNodeIndices &= 0b00110011;
            break;
        case CollisionMesh::CollisionResult::FRONT:
            intersectingNodeIndices &= 0b11001100;
            break;
        case CollisionMesh::CollisionResult::COLLISION:
            break;
        default:
            LOG_ERROR("Unexpected collision result: ", (int)resultY);
            return;
    }

    switch(resultZ)
    {
        case CollisionMesh::CollisionResult::BACK:
            intersectingNodeIndices &= 0b01010101;
            break;
        case CollisionMesh::CollisionResult::FRONT:
            intersectingNodeIndices &= 0b10101010;
            break;
        case CollisionMesh::CollisionResult::COLLISION:
            break;
        default:
            LOG_ERROR("Unexpected collision result: ", (int)resultZ);
            return;
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
        _children.emplace_back(_center + CHILD_OFFSETS[i] * childSize, childSize);
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
unsigned int Octree<ElementT>::Node::getSize() const
{
    return _elements.size();
}
