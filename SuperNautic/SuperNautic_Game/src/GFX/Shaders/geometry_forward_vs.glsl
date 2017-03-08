#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 uv; //Supports 3D textures
layout (location = 2) in vec3 normal;

uniform mat4 uModel;

out VS_OUT
{
	vec3 fragPos;
	vec2 uv;
	vec3 normal;
} vs_out;

void main()
{
	//We do not use the W component of the UVs, can be changed
	vs_out.fragPos = vec3(uModel * vec4(pos, 1.0f));
	vs_out.uv		= vec2(uv.x, 1.0 - uv.y); // UV needs to be flipped.
	vs_out.normal	= vec3(uModel * vec4(normalize(normal), 0.0f));

	gl_Position = vec4(pos, 1.0f);
	
}
