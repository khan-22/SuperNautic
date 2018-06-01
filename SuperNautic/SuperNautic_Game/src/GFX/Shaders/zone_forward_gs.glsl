#version 330 core

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
	vec3	color;
	float	strength;
	vec3	fragPos;
	float	camToFragDistance;
	vec2	uv;
} gs_out;

uniform vec3 uViewPos;


void main()
{
	float temperature	= uTemperatures[int(gs_in[0].uv.z)];		
	if (temperature > -2.0)
	{
		gs_out.color	= mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), clamp(temperature, -0.01, 0.01) * 50.0 + 0.5);
		gs_out.strength = abs(temperature);
		for (int i = 0; i < 3; i++)
		{
			gs_out.uv				 = vec2(gs_in[i].uv.x, 1.0 - gs_in[i].uv.y);
			gs_out.fragPos			 = (uModel * gl_in[i].gl_Position).xyz;
			gs_out.camToFragDistance = distance(gs_out.fragPos, uViewPos); 
			gl_Position				 = uMVP * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
	
	
}
