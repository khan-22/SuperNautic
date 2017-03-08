#include "RenderPass.hpp"

GFX::RenderPass::RenderPass()
{
	_time = 0.f;
}

GFX::RenderPass::~RenderPass()
{

}

void GFX::RenderPass::initialize(GLfloat x, GLfloat y, GLfloat width, GLfloat height, Framebuffer* resultFramebuffer, std::string shaderName)
{
	_shader = ShaderCache::get(shaderName);
	_shader.get()->bind();
	_shader.get()->setSampler("uOutsideDepthBuffer", 1);


	_resultFramebuffer = resultFramebuffer;

	_screenQuad.initialize(x, y, width, height);
}

void GFX::RenderPass::perform()
{
	_resultFramebuffer->bindRead();
	_resultFramebuffer->bindColorTextures();
	_resultFramebuffer->bindDepthTexture(1);

	Framebuffer::DEFAULT.bindWrite();

	_shader.get()->bind();
	_screenQuad.render();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*_resultFramebuffer->bindRead();
	Framebuffer::DEFAULT.bindWrite();
	LOG_GL_ERRORS();

	glBlitFramebuffer(
		0, 0, 1920, 1080,
		0, 0, 1920, 1080,
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);
	LOG_GL_ERRORS();

	

	Framebuffer::DEFAULT.bindBoth();
	LOG_GL_ERRORS();*/
	
}
