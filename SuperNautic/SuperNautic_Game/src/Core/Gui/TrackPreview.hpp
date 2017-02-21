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

    void setTrack(Track& track);

    void setSize(float size);
    void setModelMatrix(const glm::mat4& matrix);

private:
    GFX::Line _track;
    glm::vec3 _center;
    float _radius = 0.f;
    glm::mat4 _modelMatrix;
};


#endif //TRACK_PREVIEW_HPP
