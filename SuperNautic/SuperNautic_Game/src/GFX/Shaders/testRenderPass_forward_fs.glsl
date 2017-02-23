#version 400 core

out vec4 outColor;

uniform sampler2D uOrigin;

in vec2 vs_uv;

vec4 invertColor(vec4 color)
{
	return vec4(vec3(1.0) - color.rgb, 1.0);
}

void main()
{
	outColor = invertColor(texture(uOrigin, vs_uv));
}