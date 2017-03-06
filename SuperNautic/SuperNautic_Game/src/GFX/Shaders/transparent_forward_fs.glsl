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
} fs_in;

void main()
{
	vec4 diffuse	  = texture(uDiffuse,		fs_in.uv);
	vec4 specular	  = texture(uSpecular,		fs_in.uv);
	vec4 normal		  = texture(uNormal,		fs_in.uv);
	vec4 illumination = texture(uIllumination,	fs_in.uv);

	outColor		= diffuse;
}