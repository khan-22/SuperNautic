#include <cassert>
#include <limits>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"


#include "Core/Gui/GuiContainer.hpp"
#include "Core/Gui/GuiElement.hpp"


GuiContainer::GuiContainer(sf::Keyboard::Key nextKey, sf::Keyboard::Key previousKey)
: _nextKey(nextKey)
, _previousKey(previousKey)
{
    _background.setFillColor(sf::Color::Transparent);
}

GuiContainer::GuiContainer(std::vector<std::unique_ptr<GuiElement>>& elements, sf::Keyboard::Key nextKey, sf::Keyboard::Key previousKey)
: _nextKey(nextKey)
, _previousKey(previousKey)
{
    _background.setFillColor(sf::Color::Transparent);
    insert(elements);
}

GuiContainer::~GuiContainer()
{
    _elements.clear();
}

void GuiContainer::updateCurrent()
{
    for(std::unique_ptr<GuiElement>& element : _elements)
    {
        element->update();
    }
}

void GuiContainer::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_background, states);
    for(const std::unique_ptr<GuiElement>& element : _elements)
    {
        target.draw(*element, states);
    }
}

bool GuiContainer::bHasSelection() const
{
    return _selection < _elements.size();
}

void GuiContainer::handleEventCurrent(const sf::Event& event)
{
    if(_elements.empty())
    {
        return;
    }

    if(event.type == sf::Event::KeyPressed)
    {
        if(bHasSelection() && getSelection().bIsActive())
        {
            getSelection().handleEvent(event);
            if(!getSelection().bIsActive())
            {
                toggleActivation();
            }
            return;
        }


        switch(event.key.code)
        {
            case sf::Keyboard::Return:
            case sf::Keyboard::A:
                if(!bIsActive())
                {
                    toggleActivation();
                }
                break;

            case sf::Keyboard::Escape:
            case sf::Keyboard::B:
                if(bIsActive())
                {
                    toggleActivation();
                }
                break;

            default:
                if(event.key.code == _nextKey)
                {
                    selectNext();
                }
                else if(event.key.code == _previousKey)
                {
                    selectPrevious();
                }
                else if(bHasSelection())
                {
                    GuiElement& selection = getSelection();
                    if(!selection.bIsSelected())
                    {
                        selection.toggleSelection();
                    }
                    selection.handleEvent(event);
                }
                break;
        }

        return;
    }
}

GuiElement& GuiContainer::getSelection()
{
    assert(bHasSelection());
    return *_elements[_selection];
}

void GuiContainer::activateSelection()
{
    if(!bHasSelection())
    {
        return;
    }

    GuiElement& selection = getSelection();

    if(selection.bIsActivatable())
    {
        if(!selection.bIsSelected())
        {
            selection.toggleSelection();
        }

        selection.toggleActivation();

        if(!selection.bIsActive())
        {
            toggleActivation();
        }
    }
    else
    {
        toggleActivation();
    }
}

void GuiContainer::select()
{
    if(_elements.empty())
    {
        return;
    }

    if(!bHasSelection())
    {
        selectNext();
    }
    else if(!getSelection().bIsSelected())
    {
        getSelection().toggleSelection();
    }
}

void GuiContainer::deselect()
{
    if(bHasSelection() && getSelection().bIsSelected())
    {
        getSelection().toggleSelection();
    }
}

void GuiContainer::selectNext()
{
    if(_elements.empty())
    {
        return;
    }

    if(!bHasSelection())
    {
        _selection = 0;
        getSelection().toggleSelection();
        onElementSelect();
        return;
    }


    getSelection().toggleSelection();

    size_t previousSelection = _selection;
    unsigned int iteration = 1;
    do
    {
        _selection = (_selection + 1) % _elements.size();
        iteration++;
    } while(!getSelection().bIsSelectable() && iteration < _elements.size());

    getSelection().toggleSelection();


    if(previousSelection != _selection)
    {
        onElementSelect();
    }
}

void GuiContainer::selectPrevious()
{
    if(_elements.empty())
    {
        return;
    }

    if(!bHasSelection())
    {
        _selection = _elements.size() - 1;
        getSelection().toggleSelection();
        onElementSelect();
        return;
    }

    getSelection().toggleSelection();

    size_t previousSelection = _selection;
    unsigned int iteration = 1;
    do
    {
        _selection = std::min(_elements.size() - 1, _selection - 1);
        iteration++;
    } while(!getSelection().bIsSelectable() && iteration < _elements.size());
    getSelection().toggleSelection();

    if(previousSelection != _selection)
    {
        onElementSelect();
    }
}

void GuiContainer::insert(std::unique_ptr<GuiElement>& element)
{
    _elements.push_back(std::move(element));
    updateSize();
}

void GuiContainer::insert(std::vector<std::unique_ptr<GuiElement>>& elements)
{
    if(!elements.empty())
    {
        for(std::unique_ptr<GuiElement>& element : elements)
        {
            _elements.push_back(std::move(element));
        }
        updateSize();
    }

}

void GuiContainer::setOnElementSelect(const std::function<void(GuiElement*)>& func)
{
    _onElementSelectCallback = func;
}


void GuiContainer::updateSize()
{
    sf::Vector2f min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    sf::Vector2f max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for(const auto& element : _elements)
    {
        sf::FloatRect bounds = element->getBoundingRect();


        min.x = std::min(min.x, bounds.left);
        min.y = std::min(min.y, bounds.top);

        max.x = std::max(max.x, bounds.left + bounds.width);
        max.y = std::max(max.y, bounds.top + bounds.height);
    }

    sf::Vector2f pos = min;
    sf::Vector2f size = max - min;
    _bounds = sf::FloatRect(pos, size);
    float bgModifier = 0.4f;
    _background.setSize(size * (1.f + bgModifier));
    _background.setPosition(pos - size * bgModifier / 2.f);
}

sf::FloatRect GuiContainer::getBoundingRect() const
{
    return getWorldTransform().transformRect(_bounds);
}

void GuiContainer::setBackground(sf::Color fillColor, sf::Color outlineColor, float outlineThickness)
{
    _background.setFillColor(fillColor);
    _background.setOutlineColor(outlineColor);
    _background.setOutlineThickness(outlineThickness);
}

void GuiContainer::activate()
{
    if(bHasSelection() && !getSelection().bIsSelected())
    {
        getSelection().toggleSelection();
    }

    activateSelection();
}

void GuiContainer::deactivate()
{
//    if(bHasSelection())
//    {
//        getSelection().toggleSelection();
//    }
}

bool GuiContainer::bIsActivatable() const
{
    return true;
}

void GuiContainer::onElementSelect()
{
    _onElementSelectCallback(&getSelection());
}

bool GuiContainer::select(GuiElement* element)
{
    for(size_t i = 0; i < _elements.size(); i++)
    {
        if(_elements[i].get() == element)
        {
            if(!bHasSelection())
            {
                _selection = i;
                getSelection().toggleSelection();
                onElementSelect();
                return true;
            }

            size_t previousSelection = _selection;

            getSelection().toggleSelection();
            _selection = i;
            getSelection().toggleSelection();

            if(previousSelection != _selection)
            {
                onElementSelect();
            }

            return true;
        }
    }

    return false;
}
