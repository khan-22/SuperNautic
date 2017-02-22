#version 400 core

layout(location = 0) out vec3 positionOut;
layout(location = 1) out vec4 diffuseOut;
layout(location = 2) out vec3 normalOut;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;
uniform sampler2D uIllumination;

in GS_OUT
{
	vec3 fragPos;
	vec2 uv;
	vec3 normal;
} fs_in;

void main()
{
	positionOut		= fs_in.fragPos;
	normalOut		= normalize(fs_in.normal);

	vec4 diffuse	  = texture(uDiffuse, fs_in.uv);
	vec4 specular	  = texture(uSpecular, fs_in.uv);
	vec4 normal		  = texture(uNormal, fs_in.uv);
	vec4 illumination = texture(uIllumination, fs_in.uv);
	
	
	diffuseOut += (specular + normal).rgb * 0.f;
	diffuseOut		= vec4(diffuse.rgb, illumination.r);

}