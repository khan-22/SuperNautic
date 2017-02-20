#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv; //Supports 3D textures

uniform mat4 uMVP;
uniform mat4 uModel;

out VS_OUT
{
	vec3 position;
	vec2 uv;
} vs_out;

void main()
{
	//Just to prevent it from spamming errors
	mat4 test = uModel;
	mat4 test2 = uMVP;
	//
	//We do not use the W component of the UVs, can be changed
	vs_out.position = pos;
	vs_out.uv		= uv; 
	

	gl_Position = uMVP * vec4(pos, 1.0f);
	//gl_Position = vec4(pos, 1.0f);

}