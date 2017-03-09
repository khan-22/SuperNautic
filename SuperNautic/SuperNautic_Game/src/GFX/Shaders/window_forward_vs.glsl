//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 400 core

layout (location = 0) in vec3 pos;


uniform mat4 uModel;
uniform mat4 uMVP;
uniform vec3 uCameraPos;

out VS_OUT
{
	vec3 camToFragVector;
	float camToFragDistance;
} vs_out;



void main()
{
	vs_out.camToFragVector = (uModel * vec4(pos, 1.0) - uCameraPos).xyz;
	vs_out.camToFragDistance = length(vs_out.camToFragVector);

	gl_Position = uMVP * vec4(pos, 1.0);
}
