#include "GFX/Rendering/ViewportPipeline.hpp"

GFX::ViewportPipeline::ViewportPipeline()
{
}

GFX::ViewportPipeline::~ViewportPipeline()
{
}

void GFX::ViewportPipeline::initialize(sf::RenderWindow * window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer)
{
	_window = window;
	_resultFramebuffer = resultFramebuffer;
	_darkZonePass.initialize(x, y, width, height, resultFramebuffer, "darkpass_forward");
	
	_darkZonePass.setEffectFactor(0.0f);

	generalDeferred.initialize(window, x, y, width, height, resultFramebuffer);
	zoneForward.initialize(window, x, y, width, height, resultFramebuffer);
	windowForward.initialize(window, x, y, width, height, resultFramebuffer);
	transparentForward.initialize(window, x, y, width, height, resultFramebuffer);
	particleForward.initialize(window, x, y, width, height, resultFramebuffer);
}

void GFX::ViewportPipeline::setDarkFactor(float factor)
{
	_darkZonePass.setEffectFactor(factor);
}

void GFX::ViewportPipeline::display(Camera& camera)
{
	generalDeferred.display(camera);
	generalDeferred.blitDepthOnto(*_resultFramebuffer);

	zoneForward.display(camera);
	windowForward.display(camera);
	transparentForward.display(camera);
	particleForward.display(camera);

	_darkZonePass.perform();

}
