#pragma once

#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "GFX/Rendering/ScreenQuad.hpp"
#include "GFX/Rendering/Framebuffer.hpp"
#include "Core/Asset/LoadAssetFunctions.hpp"

#include <GL/glew.h>

namespace GFX
{
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();

		void initialize(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer, std::string shaderName);

		void perform();

	private:
		Framebuffer*	_resultFramebuffer;

		ShaderAsset		_shader;
		ScreenQuad		_screenQuad;

		float _time;
	};


}

#endif //RENDER_PASS_HPP