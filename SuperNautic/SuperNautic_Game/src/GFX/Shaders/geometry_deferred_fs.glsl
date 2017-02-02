#version 400 core

//out vec4 OutColor;

uniform vec4 uColor;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
} fs_in;

layout(location = 0) out vec3 positionOut;
layout(location = 1) out vec3 diffuseOut;
layout(location = 2) out vec3 normalOut;

vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	positionOut = fs_in.position;
	diffuseOut	= fs_in.uv.xyx;
	normalOut	= normalize(fs_in.normal);

	//Just output a basic color
	//lightDir = normalize(lightDir);

	//float factor = max(dot(normalize(fs_in.normal), -lightDir), 0.01);

	//OutColor = (uColor + fs_in.uv.xyxy) * factor; 
}