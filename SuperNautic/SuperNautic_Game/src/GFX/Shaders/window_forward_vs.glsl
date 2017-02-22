//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 400 core

layout (location = 0) in vec3 pos;
//layout (location = 1) in vec3 uv; //Supports 3D textures
//layout (location = 2) in vec3 normal;

//uniform mat4 uProjection;
//uniform mat4 uView;
uniform mat4 uModel;

uniform mat4 uMVP;

uniform vec3 cameraPos;

out VS_OUT
{
	vec3 direction;
	//vec2 uv;
	//vec3 normal;
} vs_out;



void main()
{
	//We do not use the W component of the UVs, can be changed
	//vs_out.uv	  = vec2(uv.x, 1.0 - uv.y); 
	//vs_out.normal = normal;

	vs_out.direction = cameraPos - (uModel * vec4(pos, 1.0f)).xyz;
	gl_Position = uMVP * vec4(pos, 1.0f);
	
}