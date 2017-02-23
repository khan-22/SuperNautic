#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in float size;

uniform mat4 uView;
uniform mat4 uModel;

out vec3 particleColor;
out float particleSize;

void main()
{
	particleColor = color;
	particleSize = size;
	gl_Position = uView * uModel * vec4(pos, 1.0);
}