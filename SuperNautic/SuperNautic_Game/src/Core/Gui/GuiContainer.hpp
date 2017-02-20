#pragma once

#ifndef GUI_CONTAINER_HPP
#define GUI_CONTAINER_HPP


#include <memory>
#include <list>


#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiElement.hpp"


class GuiContainer : public GuiElement
{
public:
    GuiContainer(sf::Keyboard::Key nextKey = sf::Keyboard::Down, sf::Keyboard::Key previousKey = sf::Keyboard::Up);

    GuiContainer(std::list<std::unique_ptr<GuiElement>>& elements, sf::Keyboard::Key nextKey = sf::Keyboard::Down, sf::Keyboard::Key previousKey = sf::Keyboard::Up);
    ~GuiContainer();

    void update();
    virtual bool bIsActivatable() const override;
    virtual void insert(std::unique_ptr<GuiElement>& element);
    virtual void insert(std::list<std::unique_ptr<GuiElement>>& elements);
    sf::FloatRect getBoundingRect() const override;
    void setBackground(sf::Color fillColor, sf::Color outlineColor = sf::Color::White, float outlineThickness = 0.f);

    void setOnElementSelect(const std::function<void(GuiElement*)>& func);

protected:
    std::list<std::unique_ptr<GuiElement>> _elements;

private:
    std::list<std::unique_ptr<GuiElement>>::iterator _selection;
    std::function<void(GuiElement*)> _onElementSelectCallback = [](GuiElement*){};
    sf::FloatRect _bounds;
    sf::RectangleShape _background;
    sf::Keyboard::Key _nextKey;
    sf::Keyboard::Key _previousKey;

    void handleEventCurrent(const sf::Event& event) override;
    void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateSize();
    void selectNext();
    void selectPrevious();
    void activateSelection();
    virtual void activate() override;
    virtual void deactivate() override;
};


#endif //GUI_CONTAINER_HPP
