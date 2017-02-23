#pragma once

#ifndef TRACK_PREVIEW_HPP
#define TRACK_PREVIEW_HPP

#include "GFX/Rendering/Line.hpp"
#include "GFX/Rendering/Renderable3D.hpp"

class Track;

class TrackPreview : public GFX::Renderable3D
{
public:
    TrackPreview();
    TrackPreview(Track& track);

    void render(GFX::RenderStates& states) override;

    void update(float dtSeconds);
    void setTrack(Track& track);
    void move(const glm::vec2& xy);

private:
    GFX::Line _track;
};


#endif //TRACK_PREVIEW_HPP
