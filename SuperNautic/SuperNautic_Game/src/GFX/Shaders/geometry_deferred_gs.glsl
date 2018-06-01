#version 330 core

layout(triangles) in;
layout(triangle_Strip, max_vertices = 6) out;

uniform mat4 uModel;
uniform mat4 uMVP;

uniform vec3 uViewPos;

in VS_OUT
{
	vec3 fragPos;
	vec2 uv;
	vec3 normal;
} gs_in[];

out GS_OUT
{
	vec3 fragPos;
	vec2 uv;
	vec3 normal;
} gs_out;

float backfaceCull(vec3 normal)
{
	float bfacecull = dot(gs_in[0].fragPos - uViewPos, (uModel * vec4(normal, 0.0f)).xyz);

	return bfacecull;
}


void main()
{
	//Calculate normal that we use for backface culling
	vec3 a = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz; //Edge 1
	vec3 b = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz; //Edge 2
	vec3 normal = normalize(cross(a, b));

	if(backfaceCull(normal) >= 0.0)
	{
		for (int i = 0; i < 3; i++)
		{
			gs_out.uv		= gs_in[i].uv;
			gs_out.normal	= gs_in[i].normal;
			gs_out.fragPos	= gs_in[i].fragPos;
			gl_Position		= uMVP * gl_in[i].gl_Position;
			EmitVertex();
		}
	}
}
