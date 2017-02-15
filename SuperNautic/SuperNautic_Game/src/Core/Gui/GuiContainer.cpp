#include <cassert>
#include <limits>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"


#include "Core/Gui/GuiContainer.hpp"
#include "Core/Gui/GuiElement.hpp"


GuiContainer::GuiContainer()
: _selection(_elements.end())
{
    _background.setFillColor(sf::Color::Transparent);
}

GuiContainer::GuiContainer(std::list<std::unique_ptr<GuiElement>>& elements)
: _selection(_elements.end())
{
    _background.setFillColor(sf::Color::Transparent);
    insert(elements);
}

GuiContainer::~GuiContainer()
{
    _elements.clear();
}

void GuiContainer::update()
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

void GuiContainer::handleEventCurrent(const sf::Event& event)
{
    if(!_elements.empty() && !(*_selection)->bIsActive())
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(!_elements.empty())
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Return:
					case sf::Keyboard::A:
                        activate();
                        break;
                    case sf::Keyboard::Down:
                        selectNext();
                        break;
                    case sf::Keyboard::Up:
                        selectPrevious();
                        break;

                    default:
                        break;
                }
            }
        }
    }
    else
    {
        (*_selection)->handleEvent(event);
    }
}

void GuiContainer::activate()
{
    if(!_elements.empty() && (*_selection)->bIsActivatable())
    {
        (*_selection)->toggleActivation();
    }
}

void GuiContainer::selectNext()
{
    if(!_elements.empty())
    {
        if(_selection != _elements.end())
        {
            (*_selection)->toggleSelection();
        }

        unsigned int iteration = 1;
        do
        {
            if(_selection == _elements.end())
            {
                _selection = _elements.begin();
            }
            else
            {
                _selection++;
                if(_selection == _elements.end())
                {
                    _selection = _elements.begin();
                }
            }

            iteration++;
        } while(!(*_selection)->bIsSelectable() && iteration < _elements.size());

        (*_selection)->toggleSelection();
    }
}

void GuiContainer::selectPrevious()
{
    if(!_elements.empty())
    {
        if(_selection != _elements.end())
        {
            (*_selection)->toggleSelection();
        }

        unsigned int iteration = 1;
        do
        {
            if(_selection == _elements.begin())
            {
                _selection = _elements.end();
                _selection--;
            }
            else
            {
                _selection--;
            }

            iteration++;
        } while(!(*_selection)->bIsSelectable() && iteration < _elements.size());


        (*_selection)->toggleSelection();
    }
}

void GuiContainer::insert(std::unique_ptr<GuiElement>& element)
{
    _elements.push_back(std::move(element));

    if(_selection != _elements.end())
    {
        (*_selection)->toggleSelection();
    }

    _selection = _elements.end();
    selectNext();
    updateSize();
}

void GuiContainer::insert(std::list<std::unique_ptr<GuiElement>>& elements)
{
    if(!elements.empty())
    {
        for(std::unique_ptr<GuiElement>& element : elements)
        {
            _elements.push_back(std::move(element));
        }

        if(_selection != _elements.end())
        {
            (*_selection)->toggleSelection();
        }

        _selection = _elements.end();
        selectNext();
        updateSize();
    }

}

void GuiContainer::updateSize()
{
    sf::Vector2f min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    sf::Vector2f max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for(std::unique_ptr<GuiElement>& element : _elements)
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
    setOrigin(size / 2.f);
    _background.setSize(size * 1.1f);
    _background.setPosition(pos - size * 0.05f);
}

sf::FloatRect GuiContainer::getBoundingRect() const
{
    return getTransform().transformRect(_bounds);
}

void GuiContainer::setBackground(sf::Color fillColor, sf::Color outlineColor, float outlineThickness)
{
    _background.setFillColor(fillColor);
    _background.setOutlineColor(outlineColor);
    _background.setOutlineThickness(outlineThickness);
}