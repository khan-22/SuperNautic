#version 330 core

layout(triangles) in;
layout(triangle_Strip, max_vertices = 3) out;

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
	float camToFragDistance;
	//vec3 tangent;
	//vec3 bitangent;
} gs_out;

float backfaceCull(vec3 normal)
{
	return dot(gs_in[0].fragPos - uViewPos, normal);
}


void main()
{
	//Calculate normal that we use for backface culling
	vec3 a = gs_in[1].fragPos - gs_in[0].fragPos;
	vec3 b = gs_in[2].fragPos - gs_in[0].fragPos;
	vec3 normal = cross(b, a);

	if(!(backfaceCull(normal) < 0.0))
	{
		normal = normalize(normal);
		
		//vec2 deltaUv1 = gs_in[1].uv - gs_in[0].uv;
		//vec2 deltaUv2 = gs_in[2].uv - gs_in[0].uv;
	
		//gs_out.tangent = normalize(a * deltaUv2.y - b * deltaUv1.y);
		//gs_out.bitangent = normalize(b * deltaUv1.x - a * deltaUv2.x);
		for (int i = 0; i < 3; i++)
		{
			gs_out.uv		= gs_in[i].uv;
			gs_out.normal	= gs_in[i].normal;
			gs_out.fragPos	= gs_in[i].fragPos;
			gs_out.camToFragDistance = distance(gs_out.fragPos, uViewPos);
	
			gl_Position		= uMVP * gl_in[i].gl_Position;
			EmitVertex();
		}
	}
}
