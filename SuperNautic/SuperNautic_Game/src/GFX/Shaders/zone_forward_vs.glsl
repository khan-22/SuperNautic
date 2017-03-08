//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 uv; //Supports 3D textures
layout (location = 2) in vec3 normal;

//uniform mat4 uModel;

//uniform mat4 uMVP;

//out VS_OUT
//{
//	vec3	position;
//	vec3	uv;
//	vec3	normal;
//	vec3	color;
//	float	temperature;
//} vs_out;


//uniform float uTemperatures[4];

out VS_OUT
{
	vec3 uv;
	//vec3 color;
	//float temperature;
} vs_out;


void main()
{
	//vs_out.temperature = uTemperatures[int(uv.z)];
	

	//float temperature	= uTemperatures[int(uv.z)];
	//vs_out.temperature  = temperature;
	//vs_out.color		= mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), temperature / 2.0 + 0.5);
	//gl_Position			= uMVP * vec4(pos, 1.0f);
	
	vs_out.uv = uv;
	gl_Position = vec4(pos, 1.0);

}