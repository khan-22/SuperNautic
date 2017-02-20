#version 400 core

layout (location = 0) in vec3 pos;

uniform mat4 uView;


void main()
{
	gl_Position = uView * vec4(pos, 1.0);
}