#version 400 core

out vec4 outColor;

uniform sampler2D uDiffuse;
//uniform sampler2D uSpecular;
//uniform sampler2D uNormal;

//in GS_OUT
//{
//	//vec3 position;
//	//vec3 uv;
//	//vec3 normal;
//	vec3 color;
//	//int id;
//} fs_in;

in GS_OUT
{
	//vec3 position;
	//vec3 uv;
	//vec3 normal;
	vec3 color;
	//float temperature;
	//int id;
} fs_in;

//vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	//Just output a basic color
	//lightDir = normalize(lightDir);


	//if (fs_in.temperature < -2.0)
	//{
	//	discard;
	//}

	outColor = vec4(fs_in.color, 1.0);

	//outColor = vec4(
	//	float(fs_in.id == 0) + 0.5, 
	//	float(fs_in.id == 1) + 0.5, 
	//	float(fs_in.id == 2) + 0.5, 
	//	1.0);
	

}