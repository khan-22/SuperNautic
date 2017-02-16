template<typename ElementT>
void Octree<ElementT>::insert(const CollisionMesh& mesh, const ElementT& element)
{
    if(bHasChildren())
    {
        for(Octree* child : getIntersectingChildren(mesh))
        {
            child->insert(mesh, element);
        }
        return;
    }

    if(getSize() + 1 >= _MAX_ELEMENTS)
    {
        _elements.emplace_back(mesh, element);
        split();
    }
}

template<typename ElementT>
bool Octree<ElementT>::bInsertIfNoCollision(const CollisionMesh& mesh, const ElementT& element)
{

}

template<typename ElementT>
void Octree<ElementT>::split()
{

}

template<typename ElementT>
bool Octree<ElementT>::bHasChildren() const
{
    return !_children.empty();
}

template<typename ElementT>
unsigned int Octree<ElementT>::getSize() const
{
    return _elements.size();
}

template<typename ElementT>
const std::vector<Octree<ElementT>*>& Octree<ElementT>::getIntersectingChildren(const CollisionMesh& mesh) const
{

}
