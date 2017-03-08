#include "SFML/Graphics/RenderTarget.hpp"

#include <cassert>

#include "Core/Gui/GuiHorizontalList.hpp"
#include "Core/Io/Log.hpp"

GuiHorizontalList::GuiHorizontalList(size_t maxVisibleElements)
: GuiContainer(sf::Keyboard::Right, sf::Keyboard::Left)
, _maxVisibleElements(maxVisibleElements)
{

}


void GuiHorizontalList::select()
{
    GuiContainer::select();
    setScale(1.2f, 1.2f);
}

void GuiHorizontalList::deselect()
{
    GuiContainer::deselect();
    setScale(1.0f, 1.0f);
}


void GuiHorizontalList::handleEventCurrent(const sf::Event& event)
{
    GuiContainer::handleEventCurrent(event);
}

bool GuiHorizontalList::bIsSelectable() const
{
    return true;
}

void GuiHorizontalList::updateSize()
{
//    _marginBetweenElements = _maxElementWidth * 0.5f;
    _marginBetweenElements = 20.f;
    _elementStepX = _marginBetweenElements + _maxElementWidth;
    sf::Vector2f size(_maxElementWidth * _maxVisibleElements + _marginBetweenElements * (_maxVisibleElements - 1), _maxElementHeight);

    _bounds.left = -size.x / 2.f;
    _bounds.top = -size.y / 2.f;
    _bounds.width = size.x;
    _bounds.height = size.y;

    _background.setSize(size);
    _background.setPosition(_bounds.left, _bounds.top);
}

void GuiHorizontalList::insert(std::unique_ptr<GuiElement>& element)
{
    sf::FloatRect rect = element->getBoundingRect();
    _maxElementWidth = std::max(rect.width, _maxElementWidth);
    _maxElementHeight = std::max(rect.height, _maxElementHeight);
    updateSize();

    element->setOrigin(rect.width / 2.f, rect.height / 2.f);
    if(_elements.empty())
    {
        element->setPosition(0.f, 0.f);
        assert(_drawElements.empty());
        _drawElements.push_back(element.get());
        _elements.push_back(std::move(element));
        return;
    }


    if(_elements.size() < (_maxVisibleElements + 1) / 2)
    {
        const auto& previous = _elements.back();
        element->setPosition(previous->getPosition());
        element->move(_elementStepX, 0.f);
        _drawElements.push_back(element.get());
    }

    _elements.push_back(std::move(element));
    updateVisuals();
}

void GuiHorizontalList::insert(std::vector<std::unique_ptr<GuiElement>>& elements)
{
    for(auto& element : elements)
    {
        insert(element);
    }
}

void GuiHorizontalList::onElementSelect()
{
    GuiContainer::onElementSelect();
    updateVisuals();
}

void GuiHorizontalList::updateVisuals()
{
    _drawElements.clear();

    GuiElement* selection;
    size_t selectionId;
    if(!bHasSelection())
    {
        assert(!_elements.empty());
        selection = _elements.front().get();
        selectionId = 0;
    }
    else
    {
        selection = &getSelection();
        selectionId = _selection;
    }

    sf::Vector2f selectionScale = selection->getScale();
    sf::Vector2f selectionOrigin(selection->getBoundingRect().width / selectionScale.x, selection->getBoundingRect().height / selectionScale.y);
    selectionOrigin /= 2.f;
    selection->setOrigin(selectionOrigin);
    selection->setPosition(0.f, 0.f);
    _drawElements.push_back(selection);

    for(size_t i = selectionId + 1; i < _elements.size() && i < selectionId + (_maxVisibleElements + 1) / 2; i++)
    {
        GuiElement& right = *_elements[i];
        right.setOrigin(right.getBoundingRect().width / 2.f, right.getBoundingRect().height / 2.f);
        right.setPosition(_drawElements.back()->getPosition());
        right.move(_elementStepX, 0.f);
        _drawElements.push_back(&right);
    }


    for(int i = static_cast<int>(selectionId - 1); i >= 0 && i > int(selectionId - (_maxVisibleElements + 1) / 2); i--)
    {
        GuiElement& left = *_elements[i];
        left.setOrigin(left.getBoundingRect().width / 2.f, left.getBoundingRect().height / 2.f);
        left.setPosition(_drawElements.front()->getPosition());
        left.move(-_elementStepX, 0.f);
        _drawElements.insert(_drawElements.begin(), &left);
    }
}

void GuiHorizontalList::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_background, states);
    for(const GuiElement* e : _drawElements)
    {
        target.draw(*e, states);
    }
}


