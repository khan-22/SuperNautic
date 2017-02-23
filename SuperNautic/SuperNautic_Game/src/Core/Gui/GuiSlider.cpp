#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiSlider.hpp"
#include "Core/Utility/Utilities.hpp"

GuiSlider::GuiSlider(float min, float max, float sliderWidth, size_t numSteps, const sf::Text& leftLabel, const sf::Text& rightLabel)
: _leftLabel(leftLabel)
, _rightLabel(rightLabel)
, _sliderWidth(sliderWidth)
, _stepWidth((sliderWidth / float(numSteps - 1)) / sliderWidth)
, _min(min)
, _valueSize(max - min)
{
    deselect();

    sf::FloatRect lRect = _leftLabel.getLocalBounds();
    _leftLabel.setOrigin(lRect.left, lRect.top);
    _leftLabel.setPosition(0.f, 0.f);

    _slider.setPosition(_leftLabel.getPosition());
    _slider.move(lRect.width, _SLIDER_HEIGHT / 4.f);

    _rightLabel.setOrigin(_rightLabel.getLocalBounds().left, _rightLabel.getLocalBounds().top);
    _rightLabel.setPosition(_slider.getPosition().x, 0.f);
    _rightLabel.move(_sliderWidth, 0.f);

    _slider.setFillColor(sf::Color::Blue);

    setOrigin(_slider.getPosition() + sf::Vector2f(_sliderWidth, _SLIDER_HEIGHT) / 2.f);
    setValue(0.f);
}


void GuiSlider::setValue(float value)
{
    float previousValue = _value;
    _value = clamp(value, 0.f, 1.f);

    _slider.setSize(sf::Vector2f(_sliderWidth * _value, _SLIDER_HEIGHT));

    if(!bIsFloatEq(previousValue, _value))
    {
        _onChangeCallback(_min + _value * _valueSize);
    }
}


float GuiSlider::getValue() const
{
    return _value;
}

bool GuiSlider::bIsSelectable() const
{
    return true;
}


void GuiSlider::select()
{
    _leftLabel.setColor(sf::Color::White);
    _rightLabel.setColor(sf::Color::White);
    setScale(1.2f, 1.2f);
}

void GuiSlider::deselect()
{
    _leftLabel.setColor(sf::Color::Black);
    _rightLabel.setColor(sf::Color::Black);
    setScale(1.0f, 1.0f);
}


void GuiSlider::handleEventCurrent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Left:
                setValue(_value - _stepWidth);
                break;
            case sf::Keyboard::Right:
                setValue(_value + _stepWidth);
                break;
            default:
                break;
        }
    }
}


void GuiSlider::renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_leftLabel, states);
    target.draw(_slider, states);
    target.draw(_rightLabel, states);
}

void GuiSlider::setOnChange(const std::function<void(float)>& callback)
{
    _onChangeCallback = callback;
}


sf::FloatRect GuiSlider::getBoundingRect() const
{
    sf::FloatRect bounds;
    sf::Vector2f lPos = _leftLabel.getPosition();
    sf::FloatRect rBounds = _rightLabel.getGlobalBounds();
    bounds.left = lPos.x;
    bounds.top = lPos.y;
    bounds.width = rBounds.left + rBounds.width - lPos.x;
    bounds.height = rBounds.top + rBounds.height - lPos.y;
    return getWorldTransform().transformRect(bounds);
}

