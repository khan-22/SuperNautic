#version 400 core

//out vec4 OutColor;

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

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;

void main()
{
	positionOut = fs_in.position;
	//diffuseOut	= uColor.rgb;
	normalOut	= normalize(fs_in.normal);
	
	float factor = max(dot(normalize(fs_in.normal), -lightDir), 0.01);

	vec4 diffuse = texture(uDiffuse, fs_in.uv);
	vec4 specular = texture(uSpecular, fs_in.uv);
	vec4 normal = texture(uNormal, fs_in.uv);
	diffuseOut = (diffuse + specular + normal).rgb * factor / 3.0;

	//Just output a basic color
	//lightDir = normalize(lightDir);

	//float factor = max(dot(normalize(fs_in.normal), -lightDir), 0.01);

	//OutColor = (uColor + fs_in.uv.xyxy) * factor; 
}