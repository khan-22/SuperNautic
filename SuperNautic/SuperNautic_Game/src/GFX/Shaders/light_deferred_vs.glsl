#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv; //Supports 3D textures

out VS_OUT
{
	vec3 position;
	vec2 uv;
} vs_out;

void main()
{
	//We do not use the W component of the UVs, can be changed
	vs_out.position = pos;
	vs_out.uv		= uv; 
	

	gl_Position = vec4(pos, 1.0f);
}
