#version 400 core

out vec4 outColor;

uniform sampler2D uPosition;
uniform sampler2D uDiffuse;
uniform sampler2D uNormal;

uniform vec3 uViewPos;

in VS_OUT
{
	vec3 position;
	vec2 uv;
} fs_in;

const int NUM_LIGHTS = 32;
struct PointLightData
{
	vec3 pos[NUM_LIGHTS];
	vec3 color[NUM_LIGHTS];
	float intensity[NUM_LIGHTS];

	//Attenuation properties
	vec3 properties[NUM_LIGHTS];
};

uniform PointLightData pointLights;

vec3 calculatePointLight(int i, vec3 fragPos, vec3 diffuseTex, vec3 normal, vec3 viewDir)
{
	//Ambient
	vec3 ambientColor	= pointLights.color[i] * 0.1;

	//Diffuse color
	vec3 lightDir		= normalize(pointLights.pos[i] - fragPos);
	float angle			= max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor	= angle * pointLights.color[i];

	//Specular
	float specularStrength	= 0.5; //Should be sent in on a per object basis, possibly with specular map in the future
	vec3 reflectionDir		= reflect(-lightDir, normal);
	float specular			= pow(max(dot(viewDir, reflectionDir), 0.0), 32); //32 defines shininess and should also be sent in as a uniform most likely
	vec3 specularVec		= specularStrength * specular * pointLights.color[i];

	//Attenuation
	float d				= length(pointLights.pos[i] - fragPos); //Distance to the light source
	vec3 dvec			= vec3(1.0, d, d*d);
	float attenuation	= pointLights.intensity[i] / (dot(pointLights.properties[i], dvec));

	vec3 result	= (diffuseColor + ambientColor + specularVec) * diffuseTex * attenuation;

	return result;
	//return vec3(1.0, 0.0, 0.0) * diffuseTex;
}

void main()
{
	vec3 fragPos	= texture(uPosition, fs_in.uv).rgb;
	vec3 diffuseTex = texture(uDiffuse, fs_in.uv).rgb;
	vec3 normal		= texture(uNormal, fs_in.uv).rgb;
	vec3 viewDir	= normalize(uViewPos - fragPos);

	vec4 lightingResult = vec4(0, 0, 0, 1);

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		lightingResult.rgb += calculatePointLight(i, fragPos, diffuseTex, normal, viewDir);
	}

	outColor = vec4(diffuseTex, 1.0);
}

//vec3 lightDir = vec3(-0.2, -0.2, -0.2);

//void showPosition();
//void showDiffuse();
//void showNormal();
//void showAll();

//void main()
//{
//	vec2 uv = fs_in.uv;

//	//vec3 position = texture(uPosition, uv).rgb;
//	//vec3 diffuse = texture(uDiffuse, uv).rgb;
//	//vec3 normal = texture(uNormal, uv).rgb;

//	//float factor = max(dot(normal, -lightDir), 0.1);

//	//fs_in.position / 2 + vec3(1.0)
//	//outColor = vec4(diffuse * factor, 1.0);

//	//outColor = vec4(texture(uDiffuse, uv).rgb, 1.0);

//	if (uv.s < 0.5 && uv.t < 0.5)
//	{
//		showPosition();
//	}
//	else if (uv.s < 0.5)
//	{
//		showDiffuse();
//	}
//	else if (uv.t < 0.5)
//	{
//		showNormal();
//	}
//	else
//	{
//		showAll();
//	}

//	//outColor = vec4(normal, 1.0);
//}


//void showPosition()
//{
//	vec2 uv = fs_in.uv * 2.0;

//	vec3 position = texture(uPosition, uv).rgb;
//	outColor = vec4(position, 1.0);
//}

//void showDiffuse()
//{
//	vec2 uv = fs_in.uv * 2.0 - vec2(0.0, 1.0);

//	vec3 diffuse = texture(uDiffuse, uv).rgb;
//	outColor = vec4(diffuse, 1.0);
//}

//void showNormal()
//{
//	vec2 uv = fs_in.uv * 2.0 - vec2(1.0, 0.0);

//	vec3 normal = texture(uNormal, uv).rgb;
//	outColor = vec4(normal, 1.0);
//}

//void showAll()
//{
//	vec2 uv = fs_in.uv * 2.0 - vec2(1.0, 1.0);

//	vec3 position = texture(uPosition, uv).rgb;
//	vec3 diffuse = texture(uDiffuse, uv).rgb;
//	vec3 normal = texture(uNormal, uv).rgb;

//	float factor = max(dot(normal, -normalize(lightDir)), 0.3);
//	outColor = vec4(diffuse * factor, 1.0);
//}