#include "GFX/Rendering/Box.hpp"

#include "Core/Io/Log.hpp"


using namespace GFX;

Box::Box()
{
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

    setBox(box);
}

Box::Box(const BoundingBox& box)
{
    setBox(box);
}

void Box::render(RenderStates& states)
{
    _model->render(states);
}

void Box::setBox(const BoundingBox& box)
{
    _box = box;
    _model = generateModel();
}

std::shared_ptr<Model> Box::generateModel() const
{
    // Start in negative corner
    glm::vec3 start = _box.center;
    for(unsigned char i = 0; i < 3; i++)
    {
        start -= _box.directions[i] * _box.halfLengths[i];
    }

    glm::vec3 edges[3];
    for(unsigned char i = 0; i < 3; i++)
    {
        edges[i] = _box.directions[i] * _box.halfLengths[i] * 2.f;
    }


    std::vector<glm::vec3> vertices =
    {
        start,                                  // (-x, -y, -z)
        start + edges[1],                       // (-x, +y, -z)
        start + edges[2],                       // (-x, -y, +z)
        start + edges[2] + edges[1],            // (-x, +y, +z)
        start + edges[2] + edges[0],            // (+x, -y, +z)
        start + edges[2] + edges[0] + edges[1], // (+x, +y, +z)
        start + edges[0],                       // (+x, -y, -z)
        start + edges[0] + edges[1]             // (+x, +y, -z)
    };

    static const std::vector<GLuint> indices =
    {
//        0, 1, 2, // -x
//        2, 1, 3,
//
//        2, 3, 4, // +z
//        4, 3, 5,
//
//        4, 5, 6, // +x
//        6, 5, 7,
//
//        6, 7, 0, // -z
//        0, 7, 1,
//
//        1, 7, 3, // +y
//        3, 7, 5,
//
//        6, 0, 4, // -z
//        4, 0, 2

        2, 1, 0, // -x
        3, 1, 2,

        4, 3, 2, // +z
        5, 3, 4,

        6, 5, 4, // +x
        7, 5, 6,

        0, 7, 6, // -z
        1, 7, 0,

        3, 7, 1, // +y
        5, 7, 3,

        4, 0, 6, // -z
        2, 0, 4
    };

    std::shared_ptr<Model> model(new Model());
    VertexArrayObject& vao = model->addMesh().getVertexArrayObject();
    size_t indicesSize = sizeof(indices[0]) * indices.size();
    size_t verticesSize = sizeof(vertices[0]) * vertices.size();
    vao.addIndexBuffer(static_cast<GLsizei>(indicesSize), GL_STATIC_DRAW);
    vao.addVertexBuffer(static_cast<GLsizei>(verticesSize), GL_STATIC_DRAW);

    vao.sendDataToIndexBuffer(0, static_cast<GLsizei>(indicesSize), (void*)indices.data());
    vao.sendDataToBuffer(0, 0, 0, static_cast<GLsizei>(verticesSize), vertices.data(), 3, GL_FLOAT);

    vao.setDrawCount(static_cast<GLuint>(indices.size()));

    return model;
}
