#pragma once

#ifndef GUI_CONTAINER_HPP
#define GUI_CONTAINER_HPP


#include <memory>
#include <list>


#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
namespace sf
{
    class Event;
}


class GuiElement;
#include "SceneNode.hpp"


class GuiContainer : public SceneNode
{
public:
    GuiContainer();

    GuiContainer(std::list<std::unique_ptr<GuiElement>>& elements);
    ~GuiContainer();

    void update();
    void insert(std::unique_ptr<GuiElement>& element);
    void insert(std::list<std::unique_ptr<GuiElement>>& elements);
    sf::FloatRect getBoundingRect() const override;
    void setBackground(sf::Color fillColor, sf::Color outlineColor = sf::Color::White, float outlineThickness = 0.f);

private:
    std::list<std::unique_ptr<GuiElement>> _elements;
    std::list<std::unique_ptr<GuiElement>>::iterator _selection;
    sf::FloatRect _bounds;
    sf::RectangleShape _background;

    void handleEventCurrent(const sf::Event& event) override;
    void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateSize();
    void selectNext();
    void selectPrevious();
    virtual void activate();
};


#endif //GUI_CONTAINER_HPP
