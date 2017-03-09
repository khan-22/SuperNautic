#include "ScreenQuad.hpp"

#include <glm/glm.hpp>

GFX::ScreenQuad::ScreenQuad()
{

}

GFX::ScreenQuad::~ScreenQuad()
{

}

void GFX::ScreenQuad::initialize(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	GLfloat screenX1 = x * 2.f - 1.f;
	GLfloat screenY1 = y * 2.f - 1.f;
	GLfloat screenX2 = (x + width)  * 2.f - 1.f;
	GLfloat screenY2 = (y + height) * 2.f - 1.f;

	glm::vec3 positions[] =
	{
		{ screenX1,	screenY1, 0.0 },
		{ screenX1,	screenY2, 0.0 },
		{ screenX2, screenY1, 0.0 },
		{ screenX2, screenY2, 0.0 },
	};

	glm::vec2 texCoords[] =
	{
		{ 0.0, 0.0 },
		{ 0.0, 1.0 },
		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
	};

	GLuint indices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

	_vao.reset(new VertexArrayObject());

	GLsizei sizePositionsInBytes = sizeof(positions[0]) * 4;
	GLsizei sizeTexCoordsInBytes = sizeof(texCoords[0]) * 4;
	_vao->addVertexBuffer(sizePositionsInBytes + sizeTexCoordsInBytes, GL_STATIC_DRAW);

	_vao->sendDataToBuffer(0, 0, 0,
		sizePositionsInBytes, &positions[0], 3, GL_FLOAT);
	_vao->sendDataToBuffer(0, 1, sizePositionsInBytes,
		sizeTexCoordsInBytes, &texCoords[0], 2, GL_FLOAT);

	GLsizei sizeIndicesInBytes = sizeof(indices[0]) * 6;
	_vao->addIndexBuffer(sizeIndicesInBytes, GL_STATIC_DRAW);
	_vao->sendDataToIndexBuffer(0, sizeIndicesInBytes, &indices[0]);

	_vao->setDrawCount(6);
}

void GFX::ScreenQuad::render()
{
	_vao->render();
}
