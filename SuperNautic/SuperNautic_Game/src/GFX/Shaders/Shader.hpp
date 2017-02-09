#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

namespace GFX
{
	class Shader
	{
		public:
			Shader(GLuint shaderProgram);
			~Shader();

			void bind();

			void setUniform(const std::string& name, const GLfloat& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::vec2& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::vec3& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::vec4& data, unsigned int count = 1);
			
			void setUniform(const std::string& name, const GLint& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::ivec2& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::ivec3& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::ivec4& data, unsigned int count = 1);
			
			void setUniform(const std::string& name, const glm::mat2& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat3& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat4& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat2x3& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat3x2& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat2x4& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat4x2& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat3x4& data, unsigned int count = 1);
			void setUniform(const std::string& name, const glm::mat4x3& data, unsigned int count = 1);

			void setSampler(const std::string& name, GLint unit);

		private:
			GLuint							_shaderProgram;
			std::map<std::string, GLint>	_uniformMap;

			GLuint getUniform(const std::string& name);
	};
}


#endif
