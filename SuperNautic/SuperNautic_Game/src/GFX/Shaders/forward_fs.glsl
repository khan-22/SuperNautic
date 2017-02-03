#version 400 core

out vec4 OutColor;

uniform vec4 uColor;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;

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

	vec4 diffuse = texture(uDiffuse, fs_in.uv);
	vec4 specular = texture(uSpecular, fs_in.uv);
	vec4 normal = texture(uNormal, fs_in.uv);
	OutColor = (diffuse + specular + normal) * factor / 3.0;
	//OutColor = texture(uTexColor, fs_in.uv) * factor;
}