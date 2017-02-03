#version 400 core


in VS_OUT
{
	vec3 position;
	vec2 uv;
} fs_in;

out vec4 outColor;

uniform sampler2D uPosition;
uniform sampler2D uDiffuse;
uniform sampler2D uNormal;

vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void showPosition();
void showDiffuse();
void showNormal();
void showAll();

void main()
{
	vec2 uv = fs_in.uv;

	//vec3 position = texture(uPosition, uv).rgb;
	//vec3 diffuse = texture(uDiffuse, uv).rgb;
	//vec3 normal = texture(uNormal, uv).rgb;

	//float factor = max(dot(normal, -lightDir), 0.1);

	//fs_in.position / 2 + vec3(1.0)
	//outColor = vec4(diffuse * factor, 1.0);

	//outColor = vec4(texture(uDiffuse, uv).rgb, 1.0);

	if (uv.s < 0.5 && uv.t < 0.5)
	{
		showPosition();
	}
	else if (uv.s < 0.5)
	{
		showDiffuse();
	}
	else if (uv.t < 0.5)
	{
		showNormal();
	}
	else
	{
		showAll();
	}

	//outColor = vec4(normal, 1.0);
}


void showPosition()
{
	vec2 uv = fs_in.uv * 2.0;

	vec3 position = texture(uPosition, uv).rgb;
	outColor = vec4(position, 1.0);
}

void showDiffuse()
{
	vec2 uv = fs_in.uv * 2.0 - vec2(0.0, 1.0);

	vec3 diffuse = texture(uDiffuse, uv).rgb;
	outColor = vec4(diffuse, 1.0);
}

void showNormal()
{
	vec2 uv = fs_in.uv * 2.0 - vec2(1.0, 0.0);

	vec3 normal = texture(uNormal, uv).rgb;
	outColor = vec4(normal, 1.0);
}

void showAll()
{
	vec2 uv = fs_in.uv * 2.0 - vec2(1.0, 1.0);

	vec3 position = texture(uPosition, uv).rgb;
	vec3 diffuse = texture(uDiffuse, uv).rgb;
	vec3 normal = texture(uNormal, uv).rgb;

	float factor = max(dot(normal, -normalize(lightDir)), 0.3);
	outColor = vec4(diffuse * factor, 1.0);
}