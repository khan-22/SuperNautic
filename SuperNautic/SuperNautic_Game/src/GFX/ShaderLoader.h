#pragma once

#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <GL/glew.h>

namespace GFX
{
	struct Shader
	{
		GLuint	shaderProgram;
	};

	class ShaderLoader
	{
	public:
		ShaderLoader(const std::string& rootPath);
		~ShaderLoader();

		Shader* loadShader(std::string shaderName);


	protected:

	private:
		std::string		_rootPath;

		std::string loadSource(std::string filename) const;
		GLuint compileShader(const char* source, GLenum type) const;
		GLuint linkProgram(GLuint vs, GLuint gs, GLuint fs) const;
	};
}

#endif //SHADER_LOADER_H