#version 400 core

out vec4 outColor;


in VS_OUT
{
	vec3 camToFragVector;
	float camToFragDistance;
	vec2 uv;
} fs_in;


uniform float uFogDistance;
uniform vec3 uCameraPos;
uniform sampler2D uTexture;

vec4 fogify(vec4 inColor, float camToFragDistance, float fogDistance)
{
	float fragDist = clamp(camToFragDistance / fogDistance, 0.0, 1.0);
	return mix(inColor, vec4(0.0, 0.0, 0.0, 1.0), fragDist);
}


void main()
{
	vec3 dir = normalize(-fs_in.camToFragVector);
	float factor = max(dot(dir, -vec3(0.0, 1.0, 0.0)), 0.0);

	//outColor = vec4(mix(vec3(0.0, 0.03, 0.06), vec3(0.0, 0.6, 0.7), factor), 0.5);
	outColor = vec4(mix(vec3(0.05, 0.10, 0.206), vec3(0.0, 0.6, 0.7), factor), 0.5);
	outColor.a = 0.0;
	outColor += texture(uTexture, fs_in.uv);
	outColor = fogify(outColor, fs_in.camToFragDistance, uFogDistance);
}