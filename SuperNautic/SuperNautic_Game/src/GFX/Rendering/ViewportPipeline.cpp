#include "GFX/Rendering/ViewportPipeline.hpp"

GFX::ViewportPipeline::ViewportPipeline()
{
    setDarkFactor(0.0f);
}

GFX::ViewportPipeline::~ViewportPipeline()
{
}

void GFX::ViewportPipeline::initialize(sf::RenderWindow * window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer)
{
	_window = window;
	_resultFramebuffer = resultFramebuffer;


	generalForward.initialize(window, x, y, width, height, resultFramebuffer);
	zoneForward.initialize(window, x, y, width, height, resultFramebuffer);
	windowForward.initialize(window, x, y, width, height, resultFramebuffer);
	transparentForward.initialize(window, x, y, width, height, resultFramebuffer);
	particleForward.initialize(window, x, y, width, height, resultFramebuffer);
}

void GFX::ViewportPipeline::setDarkFactor(float factor)
{
    _fogDistance = (1.f - factor) * 1000.f;
}

void GFX::ViewportPipeline::display(Camera& camera)
{
    generalForward.setFogDistance(_fogDistance);
	generalForward.display(camera);

    zoneForward.setFogDistance(_fogDistance);
	zoneForward.display(camera);

    windowForward.setFogDistance(_fogDistance);
	windowForward.display(camera);

    transparentForward.setFogDistance(_fogDistance);
	transparentForward.display(camera);
	particleForward.display(camera);
}
