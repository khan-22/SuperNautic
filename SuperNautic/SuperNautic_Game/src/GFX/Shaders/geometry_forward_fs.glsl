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
} fs_in;

uniform vec3 uViewPos;



//Should be exposed as uniforms
const float FOG_DISTANCE = 1500.0;
const vec4 FOG_COLOR = vec4(0.3, 0.3, 0.3, 1.0);

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


void main()
{
	vec3 diffuse	  = texture(uDiffuse, fs_in.uv).rgb;
	//vec4 specular	  = texture(uSpecular, fs_in.uv);
	//vec4 normal		  = texture(uNormal, fs_in.uv);
	vec4 illumination = texture(uIllumination, fs_in.uv);
	
	
	//outColor = vec4(diffuse.rgb, illumination.r);

	
	vec3 viewDir = normalize(uViewPos - fs_in.fragPos);

	vec4 lightingResult = vec4(0, 0, 0, 1);
	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		lightingResult.rgb += calculatePointLight(i, fs_in.fragPos, diffuse, fs_in.normal, viewDir);
	}
	
	float distanceToFragment = clamp(length(fs_in.fragPos - uViewPos) / FOG_DISTANCE, 0.0, 1.0);

	outColor = mix(mix(lightingResult, vec4(diffuse, 1.0), illumination.r), FOG_COLOR, distanceToFragment);

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
