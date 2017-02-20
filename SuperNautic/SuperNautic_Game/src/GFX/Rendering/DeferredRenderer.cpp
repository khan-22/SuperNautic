#include "GFX/Rendering/DeferredRenderer.hpp"

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

	_geometryPassShader.get()->bind();
	_geometryPassShader.get()->setSampler("uDiffuse", 0);
	_geometryPassShader.get()->setSampler("uSpecular", 1);
	_geometryPassShader.get()->setSampler("uNormal", 2);

	_lightPassShader.get()->bind();
	_lightPassShader.get()->setSampler("uPosition", 0);
	_lightPassShader.get()->setSampler("uDiffuse", 1);
	_lightPassShader.get()->setSampler("uNormal", 2);

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

	_lightVolume = ModelCache::get("lightbound.blend"); //TODO: update to KMF

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

void DeferredRenderer::pushPointLight(PointLight & pointLight)
{
	_pointLights.push_back(&pointLight);
	if (_pointLights.size() > 32)
	{
		LOG_ERROR("Cannot draw more than 32 lights in a scene");
	}
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

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	geometryPass(camera, windowWidth, windowHeight);
	lightPass(camera, windowWidth, windowHeight);

	Framebuffer::DEFAULT.bindBoth();
}

void DeferredRenderer::geometryPass(Camera& camera, GLsizei width, GLsizei height)
{
	//*************************************************//
	//*************** GEOMETRY PASS *******************//
	//*************************************************//

	_frameBuffer.bindWrite();

	//glViewport(_x * windowWidth, _y * windowHeight, _width * windowWidth, _height * windowHeight);
	glViewport(0, 0, _width * width, _height * height);

	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_geometryPassShader.get()->bind();

	_geometryPassShader.get()->setUniform("uViewPos", camera.getPosition());

	for (auto drawCall : _drawCalls)
	{
		RenderStates states{ &camera , glm::mat4(1.f), _geometryPassShader.get() };

		drawCall->render(states);
	}

	_drawCalls.clear();
}

void DeferredRenderer::lightPass(Camera& camera, GLsizei width, GLsizei height)
{
	//**********************************************//
	//*************** LIGHT PASS *******************//
	//**********************************************//

	Framebuffer::DEFAULT.bindWrite();
	_frameBuffer.bindRead();
	_frameBuffer.bindColorTextures();

	Shader* lpShader = _lightPassShader.get();
	lpShader->bind();

	lpShader->setUniform("uViewPos", camera.getPosition());
	lpShader->setUniform("screenSize", glm::vec2(width, height));

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glViewport(0, 0, width, height);

	RenderStates states{ &camera , glm::mat4(1.f), _lightPassShader.get() };
	for (auto& pointLight : _pointLights)
	{
		//Awful, make this shit better
		glm::vec3 pos = pointLight->getPosition();
		glm::vec3 color = pointLight->getLightProperties().diffuseColor;
		GLfloat intensity = pointLight->getLightProperties().intensity;
		glm::vec3 attenuation = glm::vec3(pointLight->getLightProperties().constant, pointLight->getLightProperties().linear, pointLight->getLightProperties().quadratic);
		/////////////////////////////////

		glm::mat4 modelMat = glm::translate(pos) * glm::scale(glm::vec3{ pointLight->getLightRadius() });
		//glm::mat4 modelMat = glm::translate(pos) * glm::scale(glm::vec3{3.0f, 3.0f, 3.0f});

		_lightVolume.get()->setModelMatrix(modelMat);

		lpShader->setUniform("lightPos", pos);
		lpShader->setUniform("lightColor", color);
		lpShader->setUniform("lightIntensity", intensity);
		lpShader->setUniform("lightAttenuation", attenuation);

		_lightVolume.get()->render(states);
	}

	_pointLights.clear();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
}
