#pragma once

#ifndef GUI_ELEMENT_HPP
#define GUI_ELEMENT_HPP

#include <functional>

#include "Core/Gui/SceneNode.hpp"

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
