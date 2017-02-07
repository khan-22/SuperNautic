#include "Framebuffer.hpp"

#include <assert.h>

#include "../Log.hpp"

using namespace GFX;

Framebuffer Framebuffer::DEFAULT;

Framebuffer::Framebuffer()
	: _width(0), _height(0)
	, _numColorAttachments(0U)
	, _depthTexture(0U)
	, _fbo(0U)
{

}



Framebuffer::~Framebuffer()
{
	glDeleteTextures(_numColorAttachments, _textures);
	glDeleteTextures(1, &_depthTexture);

	glDeleteFramebuffers(1, &_fbo);
}

void GFX::Framebuffer::initialize(GLuint width, GLuint height, GLuint numColorAttachments, DepthType depthType)
{
	assert(numColorAttachments <= MAX_COLOR_ATTACHMENTS);

	_width  = width;
	_height = height;

	_numColorAttachments = numColorAttachments;

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	//const int ACTUAL_WIDTH	= _window->getSize().x * _normalizedX;
	//const int ACTUAL_HEIGHT	= _window->getSize().y * _normalizedY;

	glGenTextures(numColorAttachments, _textures);

	for (int i = 0; i < numColorAttachments; i++)
	{
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
	}

	glGenTextures(1, &_depthTexture);

	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, depthType, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

	// Defines which attachments will be seen as outputs in the fragment shader.
	// Here I define an amount of MAX_COLOR_ATTACHMENTS, but note that in the next gl call I am only sending the amount
	// of actual color attachments that are present!
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	glDrawBuffers(numColorAttachments, drawBuffers);

	// Now we must check if something went wrong
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("FAILED TO CREATE FRAMEBUFFER!");
	}

	// Rebinds the default for both reading and writing
	DEFAULT.bindBoth();
}

void Framebuffer::bindWrite() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}

void Framebuffer::bindRead() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
}

void Framebuffer::bindBoth() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void Framebuffer::setReadBuffer(GLuint attachment) const
{
	assert(attachment <= _numColorAttachments);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
}

void Framebuffer::setViewport() const
{
	glViewport(0, 0, _width, _height);
}

void Framebuffer::bindColorTextures() const
{
	for (int i = _numColorAttachments - 1; i >= 0; i--)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
	}
}

