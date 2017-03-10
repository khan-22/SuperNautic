#version 400 core

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;
uniform sampler2D uIllumination;

in GS_OUT
{
	vec3 fragPos;
	vec2 uv;
	vec3 normal;
	float camToFragDistance;
	//vec3 tangent;
	//vec3 bitangent;
} fs_in;

uniform vec3 uViewPos;

uniform float uFogDistance;

const int NUM_LIGHTS = 8;
struct PointLightData
{
	vec3 pos[NUM_LIGHTS];
	vec3 color[NUM_LIGHTS];
	float intensity[NUM_LIGHTS];

	//Attenuation properties
	vec3 properties[NUM_LIGHTS];
};

uniform PointLightData pointLights;


out vec4 outColor;


vec3 calculatePointLight(int i, vec3 fragPos, vec3 diffuseTex, vec3 normal, vec3 viewDir);
vec4 fogify(vec4 inColor, float camToFragDistance, float fogDistance);


void main()
{
	vec3 diffuse	  = texture(uDiffuse, fs_in.uv).rgb;
	vec4 illumination = texture(uIllumination, fs_in.uv);
	

	vec3 viewDir = normalize(uViewPos - fs_in.fragPos);

	
	//mat3 fromFaceToWorldSpace = mat3(fs_in.tangent, fs_in.bitangent, fs_in.normal);
	//vec3 normal = texture(uNormal, fs_in.uv).xyz * 2.f - 1.f;
	//normal = normalize(fromFaceToWorldSpace * normal);
	
	vec4 lightingResult = vec4(0, 0, 0, 1);
	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		lightingResult.rgb += calculatePointLight(i, fs_in.fragPos, diffuse, fs_in.normal, viewDir);
	}

	outColor = mix(lightingResult, vec4(diffuse, 1.0), illumination);
	outColor = fogify(outColor, fs_in.camToFragDistance, uFogDistance);
}



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



vec4 fogify(vec4 inColor, float camToFragDistance, float fogDistance)
{
	float fragDist = clamp(camToFragDistance / fogDistance, 0.0, 1.0);
	return mix(inColor, vec4(0.0, 0.0, 0.0, 1.0), fragDist);
}