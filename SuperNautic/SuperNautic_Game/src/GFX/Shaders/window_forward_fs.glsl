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
	float factor = dot(dir, vec3(0, 1, 0));

	outColor = mix(vec3(1), vec3(1,0,0), factor);
}