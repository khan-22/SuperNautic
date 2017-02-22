#include "glm/gtx/norm.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "Core/Gui/TrackPreview.hpp"
#include "Core/Track/Track.hpp"


TrackPreview::TrackPreview()
{

}

TrackPreview::TrackPreview(Track& track)
{
    setTrack(track);
}

void TrackPreview::render(GFX::RenderStates& states)
{
    _track.render(states);
}


void TrackPreview::setTrack(Track& track)
{
    std::vector<glm::vec3> points;
    for(size_t i = 0; i < track.getNrOfSegments(); i++)
    {
        SegmentInstance* instance = track.getInstance(i);

        for(const glm::vec3& waypoint : instance->getParent()->getWaypoints())
        {
            points.push_back(instance->getModelMatrix() * glm::vec4(waypoint, 1.f));
        }
    }

    _track.setPoints(points);
    _track.zoom(0.80f);
}

void TrackPreview::update(float dtSeconds)
{
    _track.rotate(glm::vec3(0.f, 0.1f * dtSeconds, 0.f));
}

void TrackPreview::move(const glm::vec2& xy)
{
    _track.move(xy);
}
