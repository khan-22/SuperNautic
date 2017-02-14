#include <cassert>


#include "Core/Gui/SceneNode.hpp"

SceneNode::SceneNode()
: _children()
, _parent(nullptr)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::attachChild(std::unique_ptr<SceneNode>& child)
{
    child->_parent = this;
	_children.push_back(std::move(child));
}

std::unique_ptr<SceneNode> SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(_children.begin(), _children.end(), [&] (std::unique_ptr<SceneNode>& p) { return p.get() == &node; });
	assert(found != _children.end());

	std::unique_ptr<SceneNode> result = std::move(*found);
	result->_parent = nullptr;
	_children.erase(found);
	return result;
}

void SceneNode::clearChildren()
{
    _children.clear();
}

void SceneNode::update()
{
    updateCurrent();
	updateChildren();
}

void SceneNode::updateCurrent()
{
}

void SceneNode::updateChildren()
{
	for(std::unique_ptr<SceneNode>& child : _children)
    {
        child->update();
    }
}


void SceneNode::handleEvent(const sf::Event& event)
{
    handleEventCurrent(event);
    handleEventChildren(event);
}

void SceneNode::handleEventCurrent(const sf::Event& event)
{
}

void SceneNode::handleEventChildren(const sf::Event& event)
{
    for(std::unique_ptr<SceneNode>& child : _children)
    {
        child->handleEvent(event);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
    renderCurrent(target, states);
	renderChildren(target, states);
}

void SceneNode::renderCurrent(sf::RenderTarget&, sf::RenderStates) const
{
}

void SceneNode::renderChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const std::unique_ptr<SceneNode>& child : _children)
    {
        child->draw(target, states);
    }
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for(const SceneNode* node = this; node != nullptr; node = node->_parent)
    {
		transform = node->getTransform() * transform;
    }

	return transform;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}
