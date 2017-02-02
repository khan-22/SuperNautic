#include "DeferredRenderer.hpp"

using namespace GFX;

DeferredRenderer::DeferredRenderer()
	: _screenQuad(nullptr)
{
}


DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_geometryPassShader = ShaderCache::get("geometry_deferred");
	_lightPassShader = ShaderCache::get("light_deferred");

	_window = window;
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	_frameBuffer.initialize(_width * windowWidth, _height * windowHeight, 3);

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

void DeferredRenderer::render(Renderable3D& renderable)
{
	_drawCalls.push_back(&renderable);
}

void DeferredRenderer::display(Camera& camera)
{
	assert(_window != nullptr);

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//geometryPass(camera);
	//glViewport(0, 0, windowWidth, windowHeight);
	lightPass();

	//_frameBuffer.bindRead();
	//_frameBuffer.setReadBuffer(0);
	//glBlitFramebuffer(
	//	0, 0, _width * windowWidth, _height * windowHeight,
	//	0, 0, _width * windowWidth * 0.5f, _height * windowHeight * 0.5f,
	//	GL_COLOR_BUFFER_BIT, GL_NEAREST);


	//Framebuffer::DEFAULT.bindBoth();
	//_frameBuffer.bindRead(); 
	//_frameBuffer.setReadBuffer(0);

	//glBlitFramebuffer(
	//	0,					 0,					 _width * windowWidth,	 _height * windowHeight, 
	//	_x * windowWidth,	 _y * windowHeight, (_x + _width) * windowWidth, (_y + _height) * windowHeight,
	//	GL_COLOR_BUFFER_BIT, GL_NEAREST);

	Framebuffer::DEFAULT.bindBoth();
}

void DeferredRenderer::geometryPass(Camera & camera)
{
	_frameBuffer.bindWrite();

	GLsizei windowWidth = _window->getSize().x;
	GLsizei windowHeight = _window->getSize().y;

	//glViewport(_x * windowWidth, _y * windowHeight, _width * windowWidth, _height * windowHeight);
	glViewport(0, 0, _width * windowWidth, _height * windowHeight);

	//glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_geometryPassShader.get()->bind();
	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _geometryPassShader.get() };

		drawCall->render(states);
	}

	_drawCalls.clear();
}

void DeferredRenderer::lightPass()
{
	//Framebuffer::DEFAULT.bindWrite();
	Framebuffer::DEFAULT.bindBoth();
	//_frameBuffer.bindRead();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	_lightPassShader.get()->bind();

	_screenQuad->render();
}
