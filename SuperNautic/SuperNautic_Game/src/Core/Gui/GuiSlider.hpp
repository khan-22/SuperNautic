#pragma once

#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"


#include "Core/Gui/GuiElement.hpp"

class GuiSlider : public GuiElement
{
    public:
        GuiSlider(float min, float max, float sliderWidth, size_t numSteps, const sf::Text& leftLabel, const sf::Text& rightLabel);

        bool bIsSelectable() const override;

        void setOnChange(const std::function<void(float)>& callback);
        sf::FloatRect getBoundingRect() const override;


        void setValue(float value);
        float getValue() const;

    private:
        static constexpr float _SLIDER_HEIGHT = 10.f;
        sf::Text _leftLabel;
        sf::Text _rightLabel;
        sf::RectangleShape _slider;
        float _sliderWidth;
        float _stepWidth;
        float _min;
        float _valueSize;
        float _value = 0.f;

        std::function<void(float)> _onChangeCallback = [](float){};


        void handleEventCurrent(const sf::Event& event) override;
        void renderCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        void select() override;
        void deselect() override;
};

#endif //GUI_SLIDER_HPP
