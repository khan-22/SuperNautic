#pragma once

#ifndef GUI_CONTAINER_HPP
#define GUI_CONTAINER_HPP


#include <memory>
#include <vector>
#include <functional>

#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Event.hpp"

#include "Core/Gui/GuiElement.hpp"



class GuiContainer : public GuiElement
{
public:
    GuiContainer(sf::Keyboard::Key nextKey = sf::Keyboard::Down, sf::Keyboard::Key previousKey = sf::Keyboard::Up);

    GuiContainer(std::vector<std::unique_ptr<GuiElement>>& elements, sf::Keyboard::Key nextKey = sf::Keyboard::Down, sf::Keyboard::Key previousKey = sf::Keyboard::Up);
    ~GuiContainer();

    virtual bool bIsActivatable() const override;
    virtual void insert(std::unique_ptr<GuiElement>& element);
    virtual void insert(std::vector<std::unique_ptr<GuiElement>>& elements);
    sf::FloatRect getBoundingRect() const override;
    void setBackground(sf::Color fillColor, sf::Color outlineColor = sf::Color::White, float outlineThickness = 0.f);

    void setOnElementSelect(const std::function<void(GuiElement*)>& func);

    bool select(GuiElement* element);

protected:
    std::vector<std::unique_ptr<GuiElement>> _elements;
    size_t _selection = -1;
    sf::RectangleShape _background;
    sf::FloatRect _bounds;

    virtual void onElementSelect();
    virtual void updateSize();
    bool bHasSelection() const;
    GuiElement& getSelection();
    virtual void select() override;
    virtual void deselect() override;

protected:
    virtual void handleEventCurrent(const sf::Event& event) override;
    void activateSelection();

private:

    std::function<void(GuiElement*)> _onElementSelectCallback = [](GuiElement*){};
    sf::Keyboard::Key _nextKey;
    sf::Keyboard::Key _previousKey;

    virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void updateCurrent(float dtSeconds) override;

    void selectNext();
    void selectPrevious();
    virtual void activate() override;
    virtual void deactivate() override;
};


#endif //GUI_CONTAINER_HPP
