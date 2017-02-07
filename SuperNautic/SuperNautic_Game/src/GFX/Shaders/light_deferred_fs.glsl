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

struct PointLightData
{
	vec3 pos;
	vec3 color;

	//Attenuation properties
	float constant;
	float linear;
	float quadratic;
};

const int NUM_LIGHTS = 32;
uniform PointLightData pointLights[NUM_LIGHTS];

vec4 calculatePointLight(PointLightData pLight, vec3 fragPos, vec3 diffuse, vec3 normal, vec3 viewDir)
{
	//DO COOL LIGHT SHIT

	float d = length(pLight.pos - fragPos); //Distance to the light source
	//float attenuation  = 1.0 / (pLight.constant + (pLight.linear * d) + (pLight.quadratic * d * d));
	//float attenuation  = 1.0 / (1.0 + (0.045 * d) + (0.0075 * d * d));
	//float c = pLight.constant;
	//float l = pLight.linear;
	//float q = pLight.quadratic;

	float attenuation  = 1.0 / (1.0 + (pLight.linear * d) + (pLight.quadratic * d * d));

	float factor = dot(normalize(pLight.pos- fragPos), normal);

	//vec4 result = vec4(diffuse * pLight.color * factor * attenuation, 1.0);
	vec4 result = vec4(diffuse * pLight.color * factor * attenuation, 1.0);

	return result;
}

void main()
{
	vec3 fragPos = texture(uPosition, fs_in.uv).rgb;
	vec3 diffuse = texture(uDiffuse, fs_in.uv).rgb;
	vec3 normal = texture(uNormal, fs_in.uv).rgb;
	vec3 viewDir = normalize(uViewPos - fragPos);

	vec4 lightingResult = vec4(0, 0, 0, 1);

	for(int i = 0; i < 32; i++)
	{
		lightingResult += calculatePointLight(pointLights[i], fragPos, diffuse, normal, viewDir);
	}

	lightingResult += diffuse * 0.1;

	outColor = lightingResult;
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