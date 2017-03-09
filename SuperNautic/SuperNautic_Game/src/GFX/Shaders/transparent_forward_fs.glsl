#version 400 core

out vec4 outColor;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;
uniform sampler2D uIllumination;


in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	float camToFragDistance;
} fs_in;

const float FOG_DISTANCE = 100.0;

vec4 fogify(vec4 inColor, float camToFragDistance, float fogDistance)
{
	float fragDist = clamp(camToFragDistance / fogDistance, 0.0, 1.0);
	return mix(inColor, vec4(0.0, 0.0, 0.0, 1.0), fragDist);
}


void main()
{
	vec4 diffuse = texture(uDiffuse, fs_in.uv);
	diffuse.a = 0.5;
	
	outColor = fogify(diffuse, fs_in.camToFragDistance, FOG_DISTANCE);
}