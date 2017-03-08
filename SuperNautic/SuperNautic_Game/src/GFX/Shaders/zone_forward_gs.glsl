#version 400 core

layout(triangles) in;
layout(triangle_Strip, max_vertices = 3) out;

//uniform mat4 uModel;
uniform mat4 uMVP;

//uniform vec3 uViewPos;


uniform float uTemperatures[4];
//out VS_OUT
//{
//	vec3	position;
//	vec3	uv;
//	vec3	normal;
//	vec3	color;
//	float	temperature;
//	//int		id;
//} gs_in[];

in VS_OUT
{
	vec3	uv;
	//float	temperature;
} gs_in[];

out GS_OUT
{
	vec3 color;

	//vec3 fragPos;
	//vec2 uv;
	//vec3 normal;
} gs_out;


void main()
{
	float temperature	= uTemperatures[int(gs_in[0].uv.z)];
	//vec3 color = vec3(uTemperatures[0], uTemperatures[1], uTemperatures[2]);
		
	if (temperature > -2.0)
	{
		gs_out.color	= mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), temperature / 2.0 + 0.5);
		//gs_out.color = color;

		for (int i = 0; i < 3; i++)
		{
			gl_Position		= uMVP * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
	
	
}