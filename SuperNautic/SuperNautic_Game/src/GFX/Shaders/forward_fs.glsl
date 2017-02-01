#version 400 core

out vec4 OutColor;

uniform vec4 uColor;

in VS_OUT
{
	vec2 uv;
	vec3 normal;
} fs_in;

vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	//Just output a basic color
	lightDir = normalize(lightDir);

	float factor = max(dot(normalize(fs_in.normal), -lightDir), 0.01);

	OutColor = (uColor + fs_in.uv.xyxy) * factor; 
}