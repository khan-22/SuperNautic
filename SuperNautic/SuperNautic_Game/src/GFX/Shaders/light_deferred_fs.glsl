#version 400 core


in VS_OUT
{
	vec3 position;
	vec2 uv;
} fs_in;

out vec4 outColor;

//vec3 lightDir = vec3(-0.2, -0.2, -0.2);

void main()
{
	//fs_in.position / 2 + vec3(1.0)
	outColor = vec4(fs_in.uv.xy, 1.0, 1.0);//vec4(fs_in.uv, 1.0, 1.0);
}