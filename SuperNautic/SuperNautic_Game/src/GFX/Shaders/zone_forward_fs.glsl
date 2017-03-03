#version 400 core

out vec4 outColor;

uniform sampler2D uDiffuse;
//uniform sampler2D uSpecular;
//uniform sampler2D uNormal;

in VS_OUT
{
	vec3 position;
	vec3 uv;
	vec3 normal;
	vec3 color;
	//int id;
} fs_in;

vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	//Just output a basic color
	lightDir = normalize(lightDir);


	vec4 diffuse	= texture(uDiffuse, fs_in.uv.xy);
	//vec4 specular	= texture(uSpecular, fs_in.uv);
	//vec4 normal		= texture(uNormal, fs_in.uv);
	
	//mat3 fromTriangleSpace = mat3(fs_in.tangent, fs_in.bitangent, fs_in.normal);
	//vec3 normal = texture(uNormal, fs_in.uv).xyz * 2.f - 1.f;
	//normal = fromTriangleSpace * normal;
	
	//float factor = max(dot(normalize(normal), -lightDir), 0.01);

	outColor = vec4(fs_in.color, 1.0);

	//outColor = vec4(
	//	float(fs_in.id == 0) + 0.5, 
	//	float(fs_in.id == 1) + 0.5, 
	//	float(fs_in.id == 2) + 0.5, 
	//	1.0);
	

}