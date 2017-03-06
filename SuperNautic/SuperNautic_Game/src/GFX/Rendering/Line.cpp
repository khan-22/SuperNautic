#include <numeric>
#include <algorithm>

#include "glm/gtx/norm.hpp"

#include "GFX/Rendering/Line.hpp"

#include "Core/Io/Log.hpp"
#include "Core/Utility/Utilities.hpp"

struct Camera2D
{
    float near = 0.1f;
    glm::mat4 perspective = glm::infinitePerspective(glm::radians(90.f), 1280.f / 720.f, near);

    glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 dir = glm::vec3(0.f, 0.f, -1.f);


} camera2D;

using namespace GFX;

const unsigned char TEXTURE_COLOR[] = { 255, 255, 255, 255 };

Line::Line()
	: _shader(ShaderCache::get("particle_forward"))
	, _texture(TEXTURE_COLOR, 1, 1, GL_TEXTURE_2D)
{
    setPoints({});
}

Line::Line(const std::vector<glm::vec3>& points)
	: _shader(ShaderCache::get("particle_forward"))
	, _texture(TEXTURE_COLOR, 1, 1, GL_TEXTURE_2D)
{
    setPoints(points);
}

void Line::render(RenderStates& states)
{
    GFX::Shader* shader = _shader.get();
    shader->bind();

	shader->setUniform("uView", _modelMatrix);
	shader->setUniform("uPerspective", _screenPerspectiveMatrix);

	_texture.bind(0);

    _vao->bind();
    glDrawElements(GL_POINTS, static_cast<GLsizei>(_numPoints), GL_UNSIGNED_INT, 0);
    _vao->unbind();

	_texture.unbind(0);

    states.shader->bind();
}

void Line::setPoints(const std::vector<glm::vec3>& points)
{
    _numPoints = points.size();
    _vao = generateVao(points);


    glm::vec3 sum = std::accumulate(points.begin(), points.end(), glm::vec3(0.f));
    glm::vec3 center = sum / float(points.size());

    auto furthestFromCenter = std::max_element(points.begin(), points.end(), [&center](const glm::vec3& lhs, const glm::vec3& rhs)
    {
        return glm::distance2(lhs, center) < glm::distance2(rhs, center);
    });

    float radius = 0.f;
    if(furthestFromCenter != points.end())
    {
        radius = glm::distance(*furthestFromCenter, center);
    }


    _centerTranslation = -center;
    _screenTranslation = glm::vec2(0.f);
    _rotation = glm::mat4(1.f);
    _scale = glm::vec3(1.f);
    _radius = radius;
    _zoom = 1.f;

    updateModelMatrix();
    updateScreenPerspectiveMatrix();
}

void Line::updateModelMatrix()
{
    glm::vec3 fitNear = camera2D.pos + camera2D.dir * (camera2D.near + _radius * _scale.z / _zoom);
    _modelMatrix = glm::translate(fitNear) * _rotation * glm::scale(_scale) * glm::translate(_centerTranslation);
}

void Line::updateScreenPerspectiveMatrix()
{
    _screenPerspectiveMatrix = glm::translate(glm::vec3(_screenTranslation, 0.f)) * camera2D.perspective;
}

std::shared_ptr<GFX::VertexArrayObject> Line::generateVao(const std::vector<glm::vec3>& points) const
{
    std::vector<GLuint> indices(points.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::vector<glm::vec3> colors(points.size(), glm::vec3(0.f, 255.f, 0.f));
    if(!colors.empty())
    {
        colors.front() = colors.back() = glm::vec3(255.f, 0.f, 0.f);
    }
    std::vector<float> sizes(points.size(), 50.f);
    auto vao = std::make_shared<GFX::VertexArrayObject>();
    size_t indicesSize = sizeof(indices[0]) * indices.size();
    size_t pointsSize = sizeof(points[0]) * points.size();
    size_t colorsSize = sizeof(colors[0]) * colors.size();
    size_t sizesSize = sizeof(sizes[0]) * sizes.size();
    vao->addIndexBuffer(static_cast<GLsizei>(indicesSize), GL_STATIC_DRAW);
    vao->addVertexBuffer(static_cast<GLsizei>(pointsSize + colorsSize + sizesSize), GL_STATIC_DRAW);

    vao->sendDataToIndexBuffer(0, static_cast<GLsizei>(indicesSize), (void*)indices.data());

    size_t offset = 0;
    vao->sendDataToBuffer(0, 0, static_cast<GLuint>(offset), static_cast<GLsizei>(pointsSize), (void*)points.data(), 3, GL_FLOAT);
    offset += pointsSize;

    vao->sendDataToBuffer(0, 1, static_cast<GLuint>(offset), static_cast<GLsizei>(colorsSize), (void*)colors.data(), 3, GL_FLOAT);
    offset += colorsSize;

    vao->sendDataToBuffer(0, 2, static_cast<GLuint>(offset), static_cast<GLsizei>(sizesSize), (void*)sizes.data(), 1, GL_FLOAT);

    vao->setDrawCount(static_cast<GLuint>(indices.size()));
    return vao;
}

void Line::move(const glm::vec2& xy)
{
    _screenTranslation += xy;
    updateScreenPerspectiveMatrix();
}

void Line::rotate(const glm::vec3& xyz)
{
    _rotation = glm::rotate(_rotation, xyz.x, glm::vec3(1.f, 0.f, 0.f));
    _rotation = glm::rotate(_rotation, xyz.y, glm::vec3(0.f, 1.f, 0.f));
    _rotation = glm::rotate(_rotation, xyz.z, glm::vec3(0.f, 0.f, 1.f));

    updateModelMatrix();
}

void Line::zoom(float factor)
{
    assert(!bIsFloatEq(factor, 0.f));
    _zoom *= factor;
    updateModelMatrix();
}
