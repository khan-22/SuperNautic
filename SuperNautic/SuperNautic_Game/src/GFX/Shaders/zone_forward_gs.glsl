#version 400 core

layout(triangles) in;
layout(triangle_Strip, max_vertices = 3) out;

uniform mat4 uModel;
uniform mat4 uMVP;


uniform float uTemperatures[4];


in VS_OUT
{
	vec3	uv;
} gs_in[];

out GS_OUT
{
	vec3 color;
	vec3 fragPos;
	float camToFragDistance;
} gs_out;

uniform vec3 uViewPos;


void main()
{
	float temperature	= uTemperatures[int(gs_in[0].uv.z)];		
	if (temperature > -2.0)
	{
		gs_out.color	= mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), temperature / 2.0 + 0.5);
		for (int i = 0; i < 3; i++)
		{
			gs_out.fragPos = (uModel * gl_in[i].gl_Position).xyz;
			gs_out.camToFragDistance = distance(gs_out.fragPos, uViewPos); 
			gl_Position		= uMVP * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
	
	
}