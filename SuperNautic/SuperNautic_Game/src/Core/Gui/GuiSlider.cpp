#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include "Core/Gui/GuiSlider.hpp"
#include "Core/Utility/Utilities.hpp"

GuiSlider::GuiSlider(float min, float max, float sliderWidth, size_t numSteps, const sf::Text& leftLabel, const sf::Text& rightLabel)
: _leftLabel(leftLabel)
, _rightLabel(rightLabel)
, _sliderWidth(sliderWidth)
, _stepWidth(sliderWidth / float(numSteps - 1))
, _min(min)
, _valueSize(max - min)
, _step(_valueSize / float(numSteps - 1))
{
    deselect();

    float height = std::max(_leftLabel.getGlobalBounds().height, _rightLabel.getGlobalBounds().height);

    sf::FloatRect lRect = _leftLabel.getLocalBounds();
    _leftLabel.setOrigin(lRect.left, lRect.top);
    _leftLabel.setPosition(0.f, 0.f);
    _leftLabel.setOutlineColor(sf::Color::Black);
    _leftLabel.setOutlineThickness(2.f);

    _slider.setPosition(_leftLabel.getPosition());
    _slider.move(lRect.width + 5.f, height / 4.f);

    _rightLabel.setOrigin(_rightLabel.getLocalBounds().left, _rightLabel.getLocalBounds().top);
    _rightLabel.setPosition(_slider.getPosition().x, 0.f);
    _rightLabel.move(_sliderWidth + 5.f, 0.f);
    _rightLabel.setOutlineColor(sf::Color::Black);
    _rightLabel.setOutlineThickness(2.f);


    _slider.setFillColor(sf::Color(220, 220, 220));
    _slider.setOutlineColor(sf::Color::Black);
    _slider.setOutlineThickness(2.f);

    setOrigin(_slider.getPosition() + sf::Vector2f(_sliderWidth, _SLIDER_HEIGHT) / 2.f);
    setValue(_min);
}


void GuiSlider::setValue(float value)
{
    float previousValue = _value;
    _value = clamp(value, _min, _min + _valueSize);

    size_t numSteps = static_cast<size_t>(std::fabs(_value / _step));
    _value = _step * numSteps;

    _slider.setSize(sf::Vector2f(_stepWidth * numSteps, _SLIDER_HEIGHT));

    if(!bIsFloatEq(previousValue, _value))
    {
        _onChangeCallback(_value);
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
    _leftLabel.setFillColor(sf::Color(200, 200, 90));
    _rightLabel.setFillColor(sf::Color(200, 200, 90));
    _slider.setFillColor(sf::Color(200, 200, 90));

    setScale(1.2f, 1.2f);
}

void GuiSlider::deselect()
{
    _leftLabel.setFillColor(sf::Color::White);
    _rightLabel.setFillColor(sf::Color::White);
    _slider.setFillColor(sf::Color(220, 220, 220));

    setScale(1.0f, 1.0f);
}


void GuiSlider::handleEventCurrent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Left:
                setValue(_value - _step);
                break;
            case sf::Keyboard::Right:
                setValue(_value + _step);
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

