//Note: UVs and normals are commented out since this is meant to just be a very basic shader
//just to get up and running. We'll be switching to deferred rendering asap but they are left
//in the code just in case we would like to get better shading earlier for whatever reason.

#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv; //Supports 3D textures
//layout (location = 2) in vec3 normal;

out VS_OUT
{
	vec2 uv;
} vs_out;



void main()
{
	//We do not use the W component of the UVs, can be changed
	//vs_out.uv	  = vec2(uv.x, 1.0 - uv.y); 
	//vs_out.normal = normal;

	vs_out.uv = pos.xy / 2.0 + vec2(0.5);
	gl_Position = vec4(pos, 1.0);



	//vec4 projectedPoint = uVP * uModel * vec4(pos, 1.0);
	//vs_out.uv = 2.0 * (projectedPoint.xy / projectedPoint.w) - vec2(1.0);

}