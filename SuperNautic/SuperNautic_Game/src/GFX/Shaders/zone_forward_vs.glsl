//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 uv; //Supports 3D textures
layout (location = 2) in vec3 normal;

//uniform mat4 uProjection;
//uniform mat4 uView;
uniform mat4 uModel;

uniform mat4 uMVP;

uniform float uTemperatures[4];

out VS_OUT
{
	vec3	position;
	vec3	uv;
	vec3	normal;
	vec3	color;
	//int		id;
} vs_out;

void main()
{
	//vs_out.id		= int(uv.z);
	float temperature	= uTemperatures[int(uv.z)];

	//vs_out.color	= vec3(temperature / 2.0 + 0.5);
	vs_out.color	= mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), temperature / 2.0 + 0.5);
	vs_out.normal	= normal;
	vs_out.uv		= vec3(uv.x, 1.0 - uv.y, uv.z); 

	vs_out.position = (uModel * vec4(pos, 1.0f)).xyz;
	gl_Position = uMVP * vec4(pos, 1.0f);
	
}