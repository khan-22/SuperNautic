#pragma once

#ifndef PLAY_APPLICATION_STATE_HPP
#define PLAY_APPLICATION_STATE_HPP

#include "ApplicationState.hpp"
#include "ApplicationContext.hpp"

class PlayApplicationState : public ApplicationState
{
public:
    PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    bool bRender() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;
};


#endif //PLAY_APPLICATION_STATE_HPP
