#include "ShaderLoader.h"

#include <fstream>

#include "../Log.h"

using namespace GFX;

ShaderLoader::ShaderLoader(const std::string& rootPath)
	: _rootPath(rootPath)
{
}


ShaderLoader::~ShaderLoader()
{
}

Shader* ShaderLoader::loadShader(std::string shaderName)
{
	GLuint vertexShader		= 0U;
	GLuint geometryShader	= 0U;
	GLuint fragmentShader	= 0U;

	// Will throw if not present...
	const char* vertexSource = loadSource(_rootPath + shaderName + "_vs.glsl");
	vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	
	// Will be caught if not present since geometry shaders are
	// non-essential.
	try
	{
		const char* geometrySource = loadSource(_rootPath + shaderName + "_gs.glsl");
		geometryShader = compileShader(geometrySource, GL_GEOMETRY_SHADER);
	}
	catch (const std::exception&) {}

	// Will throw if not present... (May change?)
	const char* fragmentSource = loadSource(_rootPath + shaderName + "_fs.glsl");
	fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

	return new Shader{linkProgram(vertexShader, geometryShader, fragmentShader)};
}

const char * GFX::ShaderLoader::loadSource(std::string filename) const
{
	std::ifstream inputStream(filename);
	std::string text;

	if (inputStream.is_open())
	{
		text = std::string((std::istreambuf_iterator<char>(inputStream)),
			std::istreambuf_iterator<char>());
		inputStream.close();
	}
	else
	{
		throw std::runtime_error("Failed to load shader");
	}

	return text.c_str();
}

GLuint GFX::ShaderLoader::compileShader(const char * source, GLenum type) const
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint	success = 0;
	GLchar	shaderError[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(shaderError), nullptr, shaderError);
		LOG_ERROR("Shader compilation failed:\n", shaderError);
	}

	return shader;
}

GLuint GFX::ShaderLoader::linkProgram(GLuint vs, GLuint gs, GLuint fs) const
{
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, gs);
	glAttachShader(shaderProgram, fs);

	glLinkProgram(shaderProgram);

	GLint	success = 0;
	GLchar	programError[1024] = { 0 };

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shaderProgram, sizeof(programError), nullptr, programError);
		LOG_ERROR("Shader program linking failed:\n", programError);
	}

	return shaderProgram;
}
