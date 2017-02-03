#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 uv; //Supports 3D textures
layout (location = 2) in vec3 normal;

//uniform mat4 uProjection;
//uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uMVP;

out VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
} vs_out;

void main()
{
	//We do not use the W component of the UVs, can be changed
	vs_out.position = vec3(uModel * vec4(pos, 1.0f));
	vs_out.uv		= uv.xy; 
	vs_out.normal	= vec3(uModel * vec4(normalize(normal), 0.0f));

	mat4 poop = uView;

	gl_Position = uMVP * vec4(pos, 1.0f);
	
}