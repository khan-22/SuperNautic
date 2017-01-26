#version 400 core

out vec4 OutColor;

uniform vec4 uColor;

//in VS_OUT
//{
//	vec2 uv;
//	vec3 normal;
//} fs_in;

void main()
{
	//Just output a basic color
	OutColor = uColor; 
}