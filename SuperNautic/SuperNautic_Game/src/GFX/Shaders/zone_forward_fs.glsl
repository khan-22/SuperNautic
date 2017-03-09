#version 400 core

out vec4 outColor;


in GS_OUT
{
	vec3 color;
	vec3 fragPos;
	float camToFragDistance;
} fs_in;

uniform vec3 uViewPos;

const float FOG_DISTANCE = 100.0;

vec4 fogify(vec4 inColor, float camToFragDistance, float fogDistance)
{
	float fragDist = clamp(camToFragDistance / fogDistance, 0.0, 1.0);
	return mix(inColor, vec4(0.0, 0.0, 0.0, 1.0), fragDist);
}

void main()
{
	outColor = fogify(vec4(fs_in.color, 1.0), fs_in.camToFragDistance, FOG_DISTANCE);
}