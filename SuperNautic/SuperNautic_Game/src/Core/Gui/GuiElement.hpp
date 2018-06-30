#pragma once

#ifndef GUI_ELEMENT_HPP
#define GUI_ELEMENT_HPP

#include <functional>
#include <vector>
#include <memory>

#include "SFML/Audio/Sound.hpp"

#include "Core/Gui/SceneNode.hpp"
#include "Core/Asset/Asset.hpp"

class GuiElement : public SceneNode
{
public:
    GuiElement();
    virtual ~GuiElement();

    virtual bool bIsSelectable() const;
    bool bIsSelected() const;
    void toggleSelection();
    virtual bool bIsActivatable() const;
    bool bIsActive() const;
    void toggleActivation();

    void registerOnSelect(const std::function<void()>& callback);
    void registerOnDeselect(const std::function<void()>& callback);

    // [0, 1]
    static void setStepVolume(float volume);

protected:
    static Asset<sf::SoundBuffer> _stepSoundBuffer;
    static std::unique_ptr<sf::Sound>              _stepSound;

private:
    bool _bIsSelected;
    bool _bIsActive;

    std::vector<std::function<void()>> _onSelectCallbacks;
    std::vector<std::function<void()>> _onDeselectCallbacks;

    virtual void select();
    virtual void deselect();
    virtual void activate();
    virtual void deactivate();
};


#endif //GUI_ELEMENT_HPP
