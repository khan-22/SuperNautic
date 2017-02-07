#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
} gs_in[3];


out GS_OUT
{
	vec2 uv;
	vec4 normal;
	vec3 pos;
	vec3 tangent;
	vec3 bitangent;
} gs_out;

void main()
{
	/*
	vec3 edge1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 edge2 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);

	if(dot(normalize(vec3(gl_in[0].gl_Position) - vec3(0,0,-1)), normalize(cross(edge1, edge2))) < 0.f)
		return;
	*/

	vec3 edge1 = gs_in[1].position - gs_in[0].position;
	vec3 edge2 = gs_in[2].position - gs_in[0].position;

	vec2 deltaUv1 = gs_in[1].uv - gs_in[0].uv;
	vec2 deltaUv2 = gs_in[2].uv - gs_in[0].uv;

	gs_out.tangent = normalize(edge1 * deltaUv2.y   - edge2 * deltaUv1.y);
	gs_out.bitangent = normalize(edge2 * deltaUv1.x   - edge1 * deltaUv2.x);
	gs_out.normal = normalize(vec4(cross(edge1, edge2), 0.f));

	for(int i = 0; i < gl_in.length(); i++)
	{
		gs_out.pos = gs_in[i].position;
		gs_out.uv = gs_in[i].uv;
		gl_Position = gl_in[i].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
