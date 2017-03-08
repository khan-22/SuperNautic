#pragma once

#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP


#include <list>
#include <memory>


#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Drawable.hpp"
namespace sf
{
    class Event;
}

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    SceneNode();
    virtual ~SceneNode();

    void attachChild(std::unique_ptr<SceneNode>& child);
    std::unique_ptr<SceneNode> detachChild(const SceneNode& node);

    void update(float dtSeconds);

    virtual sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;
    virtual sf::FloatRect getBoundingRect() const;

    void handleEvent(const sf::Event& event);

protected:
    void clearChildren();

private:
    std::list<std::unique_ptr<SceneNode>>  _children;
    SceneNode*      _parent;

    virtual void handleEventCurrent(const sf::Event& event);
    void handleEventChildren(const sf::Event& event);
    virtual void updateCurrent(float dtSeconds);
    void updateChildren(float dtSeconds);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void renderChildren(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif //SCENE_NODE_HPP
