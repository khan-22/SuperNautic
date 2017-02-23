#version 400 core

in vec3 finalParticleColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(finalParticleColor, 1.0);// vec4(0.8, 0.7, 0.6, 1.0);
}