#include "GFX/Rendering/ViewportPipeline.hpp"

GFX::ViewportPipeline::ViewportPipeline()
{
}

GFX::ViewportPipeline::~ViewportPipeline()
{
}

void GFX::ViewportPipeline::initialize(sf::RenderWindow * window, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	_window = window;

	generalDeferred.initialize(window, x, y, width, height);
	zoneForward.initialize(window, x, y, width, height);
	windowForward.initialize(window, x, y, width, height);
	transparentForward.initialize(window, x, y, width, height);
	particleForward.initialize(window, x, y, width, height);
}

void GFX::ViewportPipeline::display(Camera& camera)
{
	generalDeferred.display(camera);
	generalDeferred.blitDepthOnto(GFX::Framebuffer::DEFAULT);

	zoneForward.display(camera);
	windowForward.display(camera);
	transparentForward.display(camera);
	particleForward.display(camera);
}
