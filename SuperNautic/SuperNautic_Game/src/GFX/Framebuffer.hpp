#pragma once

#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include <GL/glew.h>
#include <SFML/Window.hpp>

namespace GFX
{
	class Framebuffer
	{
	public:
		static Framebuffer DEFAULT;

		enum DepthType
		{	
			INT16		= GL_DEPTH_COMPONENT16,
			INT24		= GL_DEPTH_COMPONENT24,
			INT32		= GL_DEPTH_COMPONENT32,
			FLOAT32		= GL_DEPTH_COMPONENT32F,
		};


		Framebuffer();
		~Framebuffer();
		
		void initialize(GLuint width, GLuint height, GLuint numColorAttachments, DepthType depthType = FLOAT32);

		void bindWrite() const;
		void bindRead() const;
		void bindBoth() const;

		void setReadBuffer(GLuint attachment) const;
		void setViewport() const;

	protected:


	private:
		//sf::Window*	_window;

		//float		_normalizedX;
		//float		_normalizedY;
		//float		_normalizedWidth;
		//float		_normalizedHeight;

		GLuint	_width;
		GLuint	_height;

		GLuint	_fbo;
		GLuint  _numColorAttachments;

		// Artifical limit placed for now. May be increased when needed up to max 15.
		static const unsigned int MAX_COLOR_ATTACHMENTS = 8;
		GLuint		_textures[MAX_COLOR_ATTACHMENTS];
		GLuint		_depthTexture;

		Framebuffer(const Framebuffer& other) {};
		void operator=(const Framebuffer& other) const {};
	};
}

#endif //FRAME_BUFFER_HPP
