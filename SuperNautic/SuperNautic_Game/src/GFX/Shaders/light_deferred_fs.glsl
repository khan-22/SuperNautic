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

const int NUM_LIGHTS = 4;
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
	vec3 halfwayDir			= normalize(lightDir + viewDir);  
	float specular			= pow(max(dot(normal, halfwayDir), 0.0), 256); //32 defines shininess and should also be sent in as a uniform most likely
	vec3 specularVec		= specularStrength * specular * pointLights.color[i];

	//Attenuation
	float d				= length(pointLights.pos[i] - fragPos); //Distance to the light source
	vec3 dvec			= vec3(1.0, d, d*d);
	float attenuation	= pointLights.intensity[i] / (dot(pointLights.properties[i], dvec));

	vec3 result	= (diffuseColor + ambientColor + specularVec) * diffuseTex * attenuation;

	return result;
}

void main()
{
	vec3 fragPos		= texture(uPosition, fs_in.uv).rgb;
	vec3 diffuseTex		= texture(uDiffuse, fs_in.uv).rgb;
	vec3 normal			= texture(uNormal, fs_in.uv).rgb;
	float illumination  = texture(uDiffuse, fs_in.uv).a;
	vec3 viewDir		= normalize(uViewPos - fragPos);

	vec4 lightingResult = vec4(0, 0, 0, 1);

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		lightingResult.rgb += calculatePointLight(i, fragPos, diffuseTex, normal, viewDir);
	}
	
	//outColor = vec4(diffuseTex, 1.0);
	lightingResult.rgb += vec3(0.1, 0.9, 1.0) * 0.1; //Add slight shade of blue as ambient
	outColor = mix(lightingResult, vec4(diffuseTex, 1.0), illumination);
}