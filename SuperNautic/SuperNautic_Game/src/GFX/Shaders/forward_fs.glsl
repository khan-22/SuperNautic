#version 400 core

out vec4 OutColor;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;

in GS_OUT
{
	vec2 uv;
	vec4 normal;
	vec3 pos;
	vec3 tangent;
	vec3 bitangent;
} fs_in;

vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	//Just output a basic color
	lightDir = normalize(lightDir);


	vec4 diffuse = texture(uDiffuse, fs_in.uv);
	vec4 specular = texture(uSpecular, fs_in.uv);
	//vec4 normal = texture(uNormal, fs_in.uv);
	
	mat3 fromTriangleSpace = mat3(fs_in.tangent, fs_in.bitangent, fs_in.normal);
	vec3 normal = texture(uNormal, fs_in.uv).xyz * 2.f - 1.f;
	normal = fromTriangleSpace * normal;
	
	float factor = max(dot(normalize(normal), -lightDir), 0.01);

	
	//OutColor = (diffuse + specular + normal) * factor / 3.0;
	OutColor = diffuse; // * factor + specular * 0.f;
}