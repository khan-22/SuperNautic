#version 400 core

layout(triangles) in;
layout(triangle_Strip, max_vertices = 6) out;

uniform mat4 uMVP;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
} gs_in[];

out GS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
} gs_out;

void main()
{
	for (int i = 0; i < 3; i++)
	{
		gs_out.uv		= gs_in[i].uv;
		gs_out.normal	= gs_in[i].normal;
		gs_out.position	= gs_in[i].position;
		gl_Position		= uMVP * gl_in[i].gl_Position;
		EmitVertex();
	}
}