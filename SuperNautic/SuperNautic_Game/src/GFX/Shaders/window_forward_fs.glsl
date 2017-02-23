#version 400 core

out vec4 outColor;

//uniform sampler2D uDiffuse;
//uniform sampler2D uSpecular;
//uniform sampler2D uNormal;

in VS_OUT
{
	//vec2 uv;
	//vec4 normal;
	vec3 direction;
	//vec3 tangent;
	//vec3 bitangent;
} fs_in;

//vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	vec3 dir = normalize(fs_in.direction);
	float factor = max(dot(dir, -vec3(0.0, 1.0, 0.0)), 0.0);

	outColor = vec4(mix(vec3(0.0, 0.03, 0.06), vec3(0.0, 0.6, 0.7), factor), 1.0);
}