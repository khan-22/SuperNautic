#include <numeric>

#include "GFX/Rendering/Line.hpp"

#include "Core/Io/Log.hpp"


using namespace GFX;

Line::Line()
{
    setPoints({});
}

Line::Line(const std::vector<glm::vec3>& points)
{
    setPoints(points);
}

void Line::render(RenderStates& states)
{
//    glUseProgram(0);
//    LOG_GL_ERRORS();
//
//	_vao->bind();
//    LOG_GL_ERRORS();
//
//    glColor4b(0, 255, 0, 255);
//	glDrawElements(GL_LINE_STRIP, _numPoints, GL_UNSIGNED_INT, 0);
//    LOG_GL_ERRORS();
//
//	_vao->unbind();
//    LOG_GL_ERRORS();
//
//    states.shader->bind();

//    _model->render(states);

    for(GFX::Box& b : *_boxes)
    {
        b.render(states);
    }
}

void Line::setPoints(const std::vector<glm::vec3>& points)
{
    _numPoints = points.size();
//    _model = generateModel({glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)});
//    _model = generateModel(points);
    _boxes = generateBoxes(points);
}


std::shared_ptr<std::vector<GFX::Box>> Line::generateBoxes(const std::vector<glm::vec3>& points) const
{
    auto boxes = std::make_shared<std::vector<GFX::Box>>();
    boxes->reserve(points.size());
    BoundingBox box;
    box.center = glm::vec3(0.f);
    box.directions =
    {
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
    };
    box.halfLengths =
    {
        1.f,
        1.f,
        1.f
    };

    for(const glm::vec3& p : points)
    {
        box.center = p;
        boxes->emplace_back(box);
    }

    return boxes;
}


std::shared_ptr<GFX::Model> Line::generateModel(const std::vector<glm::vec3>& points) const
{
    std::vector<glm::vec3> vertices;
    vertices.reserve(points.size() * 4);
    for(size_t i = 0, j = 1; j < points.size(); i++, j++)
    {
        vertices.push_back(points[i]);
        vertices.push_back(points[j]);
        vertices.push_back(points[i] + glm::vec3(1.f, 1.f, 1.f) * 1.f);
        vertices.push_back(points[j] + glm::vec3(1.f, 1.f, 1.f) * 1.f);
    }

    std::vector<GLuint> indices(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);


    std::shared_ptr<Model> model(new Model());
    VertexArrayObject& vao = model->addMesh().getVertexArrayObject();
    size_t indicesSize = sizeof(indices[0]) * indices.size();
    size_t verticesSize = sizeof(vertices[0]) * vertices.size();
    vao.addIndexBuffer(indicesSize, GL_STATIC_DRAW);
    vao.addVertexBuffer(verticesSize, GL_STATIC_DRAW);

    vao.sendDataToIndexBuffer(0, indicesSize, (void*)indices.data());
    vao.sendDataToBuffer(0, 0, 0, verticesSize, vertices.data(), 3, GL_FLOAT);

    vao.setDrawCount(indices.size());
    return model;
//
//    std::vector<GLuint> indices(vertices.size());
//    std::iota(indices.begin(), indices.end(), 0);
//
//    auto vao = std::shared_ptr<GFX::VertexArrayObject>(new VertexArrayObject());
//    size_t indicesSize = sizeof(indices[0]) * indices.size();
//    size_t verticesSize = sizeof(vertices[0]) * vertices.size();
//    vao->addIndexBuffer(indicesSize, GL_STATIC_DRAW);
//    vao->addVertexBuffer(verticesSize, GL_STATIC_DRAW);
//
//    vao->sendDataToIndexBuffer(0, indicesSize, (void*)indices.data());
//    vao->sendDataToBuffer(0, 0, 0, verticesSize, (void*)vertices.data(), 3, GL_FLOAT);
//
//    vao->setDrawCount(indices.size());
//
//    return vao;
}

void Line::setModelMatrix(const glm::mat4& matrix)
{
//    _model->setModelMatrix(matrix);
}
