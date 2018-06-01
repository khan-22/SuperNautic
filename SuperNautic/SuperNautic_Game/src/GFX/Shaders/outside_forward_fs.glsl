#version 330 core

out vec4 outColor;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;
uniform sampler2D uIllumination;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	float dist;
} fs_in;

vec3 lightDir = vec3(0.0, -1.0, 0.0);

void main()
{
	//Just output a basic color
	lightDir = normalize(lightDir);
	
	vec4 diffuse	  = texture(uDiffuse, fs_in.uv);
	vec4 specular	  = texture(uSpecular, fs_in.uv);
	vec4 normal		  = texture(uNormal, fs_in.uv);
	vec4 illumination = texture(uIllumination, fs_in.uv);
	
	if (diffuse.a < 0.9)
	{
		discard;
	}

	float factor = dot(lightDir, -fs_in.normal);

	vec4 shadedColor = mix(diffuse * vec4(0.2, 0.5, 0.6, 1.0), diffuse * vec4(0.06, 0.07, 0.1, 1.0), factor);

	vec4 illuminatedColor = mix(shadedColor, diffuse, illumination);

	outColor = mix(illuminatedColor, vec4(0.0, 0.0, 0.0, 1.0), fs_in.dist);
}
