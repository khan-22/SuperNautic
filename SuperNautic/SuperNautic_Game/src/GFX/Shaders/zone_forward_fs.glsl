#version 330 core

out vec4 outColor;

uniform sampler2D uPattern;

in GS_OUT
{
	vec3	color;
	float	strength;
	vec3	fragPos;
	float	camToFragDistance;
	vec2	uv;
} fs_in;

uniform vec3 uViewPos;

uniform float uFogDistance;

vec4 fogify(vec4 inColor, float camToFragDistance, float fogDistance)
{
	float fragDist = clamp(camToFragDistance / fogDistance, 0.0, 1.0);
	return mix(inColor, vec4(0.0, 0.0, 0.0, 1.0), fragDist);
}

void main()
{
	vec3 color = fs_in.color * (texture(uPattern, fs_in.uv).r + fs_in.strength);
	outColor = fogify(vec4(color, 1.0), fs_in.camToFragDistance, uFogDistance);

	//outColor = texture(uPattern, fs_in.uv);
}
