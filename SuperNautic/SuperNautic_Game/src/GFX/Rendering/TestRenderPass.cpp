#include "TestRenderPass.hpp"

GFX::TestRenderPass::TestRenderPass()
{
}

GFX::TestRenderPass::~TestRenderPass()
{
}

void GFX::TestRenderPass::initialize(sf::RenderWindow * window, Framebuffer origin, Framebuffer target, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_shader = ShaderCache::get("");

	GLfloat screenX1 = _x * 2.f - 1.f;
	GLfloat screenY1 = _y * 2.f - 1.f;
	GLfloat screenX2 = (_x + _width)  * 2.f - 1.f;
	GLfloat screenY2 = (_y + _height) * 2.f - 1.f;

	glm::vec3 positions[] =
	{
		{ screenX1, screenY1, 0.0 },
		{ screenX1, screenY2, 0.0 },
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

	_screenQuad.reset(new VertexArrayObject());

	GLsizei sizePositionsInBytes = sizeof(positions[0]) * 4;
	GLsizei sizeTexCoordsInBytes = sizeof(texCoords[0]) * 4;
	_screenQuad->addVertexBuffer(sizePositionsInBytes + sizeTexCoordsInBytes, GL_STATIC_DRAW);

	_screenQuad->sendDataToBuffer(0, 0, 0,
		sizePositionsInBytes, &positions[0], 3, GL_FLOAT);
	_screenQuad->sendDataToBuffer(0, 1, sizePositionsInBytes,
		sizeTexCoordsInBytes, &texCoords[0], 2, GL_FLOAT);

	GLsizei sizeIndicesInBytes = sizeof(indices[0]) * 6;
	_screenQuad->addIndexBuffer(sizeIndicesInBytes, GL_STATIC_DRAW);
	_screenQuad->sendDataToIndexBuffer(0, sizeIndicesInBytes, &indices[0]);

	_screenQuad->setDrawCount(6);
}

void GFX::TestRenderPass::perform()
{
	assert(_window != nullptr);


}
