#pragma once

#ifndef VIEWPORT_PIPELINE_HPP
#define VIEWPORT_PIPELINE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <GL\glew.h>

#include "GFX/Rendering/DeferredRenderer.hpp"
#include "GFX/Rendering/ZoneRenderer.hpp"
#include "GFX/Rendering/ForwardRenderer.hpp"
#include "GFX/Rendering/WindowRenderer.hpp"
#include "GFX/Rendering/TransparentRenderer.hpp"
#include "GFX/Rendering/ParticleRenderer.hpp"
#include "GFX/Rendering/RenderPass.hpp"

#include "Core/Utility/Camera.h"

namespace GFX
{
	struct ViewportPipeline
	{
	public:
		ViewportPipeline();
		~ViewportPipeline();

		void initialize(sf::RenderWindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer);

		void setDarkFactor(float factor);

		GFX::DeferredRenderer		generalDeferred;
		GFX::ZoneRenderer			zoneForward;
		GFX::WindowRenderer			windowForward;
		GFX::TransparentRenderer	transparentForward;
		GFX::ParticleRenderer		particleForward;

		void display(Camera& camera);


	private:
		sf::RenderWindow*	_window;

		Framebuffer*		_resultFramebuffer;

		RenderPass			_darkZonePass;
	};
}


#endif //VIEWPORT_PIPELINE_HPP
