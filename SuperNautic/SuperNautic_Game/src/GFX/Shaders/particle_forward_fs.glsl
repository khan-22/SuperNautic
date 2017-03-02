#version 400 core

in vec3 finalParticleColor;
in vec2 finalParticleUV;

out vec4 fragColor;

uniform sampler2D uTexture;

void main()
{
	fragColor = vec4(finalParticleColor * texture(uTexture, finalParticleUV.xy).rgb, 1.0);// vec4(0.8, 0.7, 0.6, 1.0);
}