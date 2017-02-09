#include "Shader.hpp"

#include "../../Log.hpp"

using namespace GFX;

GFX::Shader::Shader(GLuint shaderProgram)
	: _shaderProgram(shaderProgram)
{
}

GFX::Shader::~Shader()
{
	glDeleteProgram(_shaderProgram);
}

void GFX::Shader::bind()
{
	glUseProgram(_shaderProgram);
}

void GFX::Shader::setUniform(const std::string& name, const GLfloat& data, unsigned int count)
{
	glUniform1fv(getUniform(name), count, &data);
}

void GFX::Shader::setUniform(const std::string& name, const glm::vec2 & data, unsigned int count)
{
	glUniform2fv(getUniform(name), count, &data[0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::vec3 & data, unsigned int count)
{
	glUniform3fv(getUniform(name), count, &data[0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::vec4 & data, unsigned int count)
{
	glUniform4fv(getUniform(name), count, &data[0]);
}

void GFX::Shader::setUniform(const std::string& name, const GLint & data, unsigned int count)
{
	glUniform1iv(getUniform(name), count, &data);
}

void GFX::Shader::setUniform(const std::string& name, const glm::ivec2 & data, unsigned int count)
{
	glUniform2iv(getUniform(name), count, &data[0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::ivec3 & data, unsigned int count)
{
	glUniform3iv(getUniform(name), count, &data[0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::ivec4 & data, unsigned int count)
{
	glUniform4iv(getUniform(name), count, &data[0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat2 & data, unsigned int count)
{
	glUniformMatrix2fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat3 & data, unsigned int count)
{
	glUniformMatrix3fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat4 & data, unsigned int count)
{
	glUniformMatrix4fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat2x3 & data, unsigned int count)
{
	glUniformMatrix2x3fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat3x2 & data, unsigned int count)
{
	glUniformMatrix3x2fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat2x4 & data, unsigned int count)
{
	glUniformMatrix2x4fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat4x2 & data, unsigned int count)
{
	glUniformMatrix4x2fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat3x4 & data, unsigned int count)
{
	glUniformMatrix3x4fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setUniform(const std::string& name, const glm::mat4x3 & data, unsigned int count)
{
	glUniformMatrix4x3fv(getUniform(name), count, GL_FALSE, &data[0][0]);
}

void GFX::Shader::setSampler(const std::string& name, GLint unit)
{
	glUniform1i(getUniform(name), unit);
}

GLuint GFX::Shader::getUniform(const std::string& name)
{
	auto found = _uniformMap.find(name);
	if (found != _uniformMap.end())
	{
		return found->second;
	}

	GLint location = glGetUniformLocation(_shaderProgram, name.c_str());

	if (location == -1)
	{
		LOG_ERROR("Failed to get the uniform location for: ", name);
	}
	else
	{
		_uniformMap[name] = location;
	}

	return location;
}
