#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 uPerspective;

//createVertex(vec3 h, vec3 v, vec2 uv)
//{
//	vec4 point = gl_in[0].gl_Position + h + v;
//	gl_Position = uPerspective * point;



//}


void main()
{
	vec4 point = gl_in[0].gl_Position;
	vec3 cameraDir = -point.xyz;
	float particleSize = 1.f;

	vec3 horizontal = normalize(cross(cameraDir, vec3(0.0, 1.0, 0.0)) * particleSize / 2.0;
	vec3 vertical	= normalize(cross(cameraDir, horizontal) * particleSize / 2.0;

	gl_Position = uPerspective * (point - horizontal + vertical);
	EmitVertex();

	gl_Position = uPerspective * (point + horizontal + vertical);
	EmitVertex();
	
	gl_Position = uPerspective * (point - horizontal - vertical);
	EmitVertex();
	
	gl_Position = uPerspective * (point + horizontal - vertical);
	EmitVertex();

	EndPrimitive();
}