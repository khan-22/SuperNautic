#include "Log.hpp"

#include <GL/glew.h>

detail::Log* detail::Log::_instance = nullptr;

detail::Log::Log()
{
	
}

detail::Log::~Log()
{
	_file.open("log.txt");

	_file << _message.str();

	_file.close();
}

detail::Log* detail::Log::getInstance()
{
	if (!_instance)
	{
		_instance = new Log();
	}

	return _instance;
}

std::string detail::Log::printGlErrors()
{
	int e = glGetError();
	if (e == GL_NO_ERROR)
	{
		return "";
	}

	std::string errorMessage = "glGetError() reports: ";
	switch (e)
	{
	case GL_INVALID_ENUM: errorMessage.append("GL_INVALID_ENUM"); break;
	case GL_INVALID_VALUE: errorMessage.append("GL_INVALID_VALUE"); break;
	case GL_INVALID_OPERATION: errorMessage.append("GL_INVALID_OPERATION"); break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: errorMessage.append("GL_INVALID_FRAMEBUFFER_OPERATION"); break;
	case GL_OUT_OF_MEMORY: errorMessage.append("GL_OUT_OF_MEMORY"); break;
	case GL_STACK_UNDERFLOW: errorMessage.append("GL_STACK_UNDERFLOW"); break;
	case GL_STACK_OVERFLOW: errorMessage.append("GL_STACK_OVERFLOW"); break;
	}

	return errorMessage;
}

void detail::Log::close()
{
	delete _instance;
}