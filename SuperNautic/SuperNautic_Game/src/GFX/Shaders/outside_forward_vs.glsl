//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 uv; //Supports 3D textures
layout (location = 2) in vec3 normal;

//uniform mat4 uProjection;
//uniform mat4 uView;
uniform mat4 uModel;

uniform mat4 uMVP;

uniform vec3 uCameraPos;

out VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	float dist;
} vs_out;

void main()
{
	//We do not use the W component of the UVs, can be changed
	vs_out.uv	  = vec2(uv.x, 1.0 - uv.y); 
	vs_out.normal = vec3(uModel * vec4(normal, 0.0));

	vs_out.position = vec3(uModel * vec4(pos, 1.0f));

	gl_Position = uMVP * vec4(pos, 1.0f);
	
	vs_out.dist = clamp(length(uCameraPos - vs_out.position) / 1000.0, 0.0, 1.0);
}
