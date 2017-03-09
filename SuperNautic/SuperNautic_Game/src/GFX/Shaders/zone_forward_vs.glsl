//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 uv; //Supports 3D textures
layout (location = 2) in vec3 normal;


out VS_OUT
{
	vec3 uv;
} vs_out;


void main()
{
	vs_out.uv = uv;
	gl_Position = vec4(pos, 1.0);
}