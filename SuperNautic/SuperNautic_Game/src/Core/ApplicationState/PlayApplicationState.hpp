#pragma once

#ifndef PLAY_APPLICATION_STATE_HPP
#define PLAY_APPLICATION_STATE_HPP

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/World/World.hpp"

class PlayApplicationState : public ApplicationState
{
public:
    PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context);

    bool bRender() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;

private:
    World _world;
};


#endif //PLAY_APPLICATION_STATE_HPP
