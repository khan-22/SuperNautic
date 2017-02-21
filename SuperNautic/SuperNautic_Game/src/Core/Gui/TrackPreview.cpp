#include "glm/gtx/norm.hpp"

#include "Core/Gui/TrackPreview.hpp"
#include "Core/Track/Track.hpp"


TrackPreview::TrackPreview()
: _modelMatrix(1.f)
{

}

TrackPreview::TrackPreview(Track& track)
: _modelMatrix(1.f)
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
    glm::vec3 sum;
    for(size_t i = 0; i < track.getNrOfSegments(); i++)
    {
        SegmentInstance* instance = track.getInstance(i);

        for(const glm::vec3& waypoint : instance->getParent()->getWaypoints())
        {
            points.push_back(instance->getModelMatrix() * glm::vec4(waypoint, 1.f));
            sum += points.back();
        }
    }

    _center = sum / float(points.size());

    auto furthestFromCenter = std::max_element(points.begin(), points.end(), [this](const glm::vec3& lhs, const glm::vec3& rhs)
    {
        return glm::distance2(lhs, _center) < glm::distance2(rhs, _center);
    });
    if(furthestFromCenter != points.end())
    {
        _radius = glm::distance(*furthestFromCenter, _center);
    }
    _modelMatrix = glm::translate(glm::vec3(-_center.x, -_center.y, -_center.z));


    _track.setPoints(points);
    _track.setModelMatrix(_modelMatrix);
}

void TrackPreview::setSize(float size)
{
    float ratio = size / _radius;
    _radius = size;

    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(1.f, 1.f, 1.f) * ratio);
    _track.setModelMatrix(_modelMatrix);
}

//void TrackPreview::setPosition(float x, float y)
//{
////    glm::translat
////    _modelMatrix = glm::translate(_modelMatrix, )
//}
