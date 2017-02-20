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

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 lightAttenuation;

uniform vec2 screenSize;

vec3 calculatePointLight(vec3 fragPos, vec3 diffuseTex, vec3 normal, vec3 viewDir)
{
	//Ambient
	vec3 ambientColor	= lightColor * 0.1;

	//Diffuse color
	vec3 lightDir		= normalize(lightPos - fragPos);
	float angle			= max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor	= angle * lightColor;

	//Specular
	float specularStrength	= 0.5; //Should be sent in on a per object basis, possibly with specular map in the future
	vec3 reflectionDir		= reflect(-lightDir, normal);
	vec3 halfwayDir			= normalize(lightDir + viewDir);  
	float specular			= pow(max(dot(normal, halfwayDir), 0.0), 256); //256 defines shininess and should also be sent in as a uniform most likely
	vec3 specularVec		= specularStrength * specular * lightColor;

	//Attenuation
	float d				= length(lightPos - fragPos); //Distance to the light source
	vec3 dvec			= vec3(1.0, d, d*d);
	float attenuation	= lightIntensity / (dot(lightAttenuation, dvec));

	vec3 result	= (diffuseColor + ambientColor + specularVec) * diffuseTex * attenuation;

	return result;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 fragPos	= texture(uPosition, uv).rgb;
	vec3 diffuseTex = texture(uDiffuse, uv).rgb;
	vec3 normal		= texture(uNormal, uv).rgb;
	vec3 viewDir	= normalize(uViewPos - fragPos);

	vec4 lightingResult = vec4(0, 0, 0, 1.0);

	lightingResult.rgb = calculatePointLight(fragPos, diffuseTex, normal, viewDir);

	//outColor = vec4(diffuseTex, 1.0);
	outColor = lightingResult;
}