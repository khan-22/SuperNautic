#pragma once

#ifndef SHADER_LOADER_HPP
#define SHADER_LOADER_HPP

#include <string>
#include <GL/glew.h>

#include "GFX/Resources/Shader.hpp"

namespace GFX
{

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

#endif //SHADER_LOADER_HPP