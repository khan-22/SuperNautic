#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "SFML\OpenGL.hpp"

namespace GFX
{
	class Shader
	{
		public:
			GLuint getShaderProgram();

		private:
			GLuint ShaderProgram;
	};
}


#endif
