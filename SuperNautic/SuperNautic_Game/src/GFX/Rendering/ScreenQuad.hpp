#pragma once


#ifndef SCREEN_QUAD_HPP
#define SCREEN_QUAD_HPP

#include <GL/glew.h>

#include "GFX/Resources/VertexArrayObject.hpp"

namespace GFX
{
	class ScreenQuad 
	{
	public:
		ScreenQuad();
		~ScreenQuad();

		void initialize(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
		void render();


	private:
		std::unique_ptr<VertexArrayObject>		_vao;

	};
}

#endif //SCREEN_QUAD_HPP