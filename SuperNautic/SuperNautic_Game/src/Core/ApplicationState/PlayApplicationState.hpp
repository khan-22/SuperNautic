#pragma once

#ifndef PLAY_APPLICATION_STATE_HPP
#define PLAY_APPLICATION_STATE_HPP

#include "Core/ApplicationState/ApplicationState.hpp"
#include "Core/Track/SegmentHandler.hpp"
#include "Core/Track/Track.hpp"
#include "Core/World/World.hpp"

class PlayApplicationState : public ApplicationState
{
public:
    PlayApplicationState(ApplicationStateStack& stack, ApplicationContext& context, int numberOfPlayers);

    void render() override;
    bool bUpdate(float dtSeconds) override;
    bool bHandleEvent(const sf::Event& event) override;
	void initialize() override;

private:
	SegmentHandler	_segmentHandler;
	Track			_track;
    World			_world;
};


#endif //PLAY_APPLICATION_STATE_HPP
