#version 400 core

out vec4 outColor;

//uniform sampler2D uDiffuse;
//uniform sampler2D uSpecular;
//uniform sampler2D uNormal;

uniform sampler2D uOutsideBuffer;
uniform sampler2D uOutsideDepthBuffer;

in VS_OUT
{
	vec2 uv;
} fs_in;

#define SAMPLE_SIZE 3

void main()
{
	vec4 bufferColor = texture(uOutsideDepthBuffer, fs_in.uv);

	//float uvStepH = 0.0005208 * 2.0;
	//float uvStepV = 0.0009259 * 2.0;

	//float offsets[SAMPLE_SIZE];
	//int halfSample = SAMPLE_SIZE / 2;
	//for (int i = 0; i < SAMPLE_SIZE; ++i)
	//{
	//	offsets[i] = float(i - halfSample);
	//}

	//vec4 bufferSamples[SAMPLE_SIZE * SAMPLE_SIZE];
	//for (int i = 0; i < SAMPLE_SIZE; ++i)
	//{
	//	for (int j = 0; j < SAMPLE_SIZE; ++j)
	//	{
	//		bufferSamples[i + j * SAMPLE_SIZE] =  texture(uOutsideBuffer, fs_in.uv + vec2(offsets[i] * uvStepH, offsets[j] * uvStepV));
	//	}
	//}


	//vec4 bufferSamples[9] = 
	//{
	//	texture(uOutsideBuffer, fs_in.uv + vec2(-uvStepH, -uvStepV)), 
	//	texture(uOutsideBuffer, fs_in.uv + vec2(0, -uvStepV)), 
	//	texture(uOutsideBuffer, fs_in.uv + vec2(uvStepH, -uvStepV)),

	//	texture(uOutsideBuffer, fs_in.uv + vec2(-uvStepH, 0)), 
	//	texture(uOutsideBuffer, fs_in.uv + vec2(0, 0)), 
	//	texture(uOutsideBuffer, fs_in.uv + vec2(uvStepH, 0)),

	//	texture(uOutsideBuffer, fs_in.uv + vec2(-uvStepH, uvStepV)), 
	//	texture(uOutsideBuffer, fs_in.uv + vec2(0, uvStepV)), 
	//	texture(uOutsideBuffer, fs_in.uv + vec2(uvStepH, uvStepV))
	//};
	
	//for (int i = 0; i < 9; ++i)
	//{
	//	// Pow 4 on all samples
	//	bufferSamples[i] = bufferSamples[i] * bufferSamples[i] * bufferSamples[i] * bufferSamples[i] * 4.0;
	//}

	//for (int i = 0; i < SAMPLE_SIZE; ++i)
	//{
	//	for (int j = 0; j < SAMPLE_SIZE; ++j)
	//	{
	//		// Pow 4 on all samples
	//		bufferSamples[i + j * SAMPLE_SIZE] = bufferSamples[i + j * SAMPLE_SIZE]  * bufferSamples[i + j * SAMPLE_SIZE] * bufferSamples[i + j * SAMPLE_SIZE] * bufferSamples[i + j * SAMPLE_SIZE] * 2.0;
	//	}
	//}


	//float sampleWeights[SAMPLE_SIZE * SAMPLE_SIZE] =
	//{
	//	1.0f, 2.0, 1.0,
	//	2.0f, 4.0, 2.0,
	//	1.0f, 2.0, 1.0,
	//};	
	
	////float sampleWeights[SAMPLE_SIZE * SAMPLE_SIZE] =
	////{
	////	1.0f, 4.0,  7.0,  4.0,  1.0,
	////	4.0f, 16.0, 26.0, 16.0, 4.0,
	////	7.0f, 26.0, 41.0, 26.0, 7.0,
	////	4.0f, 16.0, 26.0, 16.0, 4.0,
	////	1.0f, 4.0,  7.0,  4.0,  1.0,
	////};

	//vec4 average = vec4(0.0);

	//for (int i = 0; i < SAMPLE_SIZE; ++i)
	//{	
	//	for (int j = 0; j < SAMPLE_SIZE; ++j)
	//	{
	//		average += sampleWeights[i + j * SAMPLE_SIZE] * bufferSamples[i + j * SAMPLE_SIZE];
	//	}
	//}

	//vec4 bufferColor = texture(uOutsideBuffer, vec2(fs_in.uv.x + sin(uTime + fs_in.uv.y * 20.0) * 0.01, fs_in.uv.y));

	//outColor = vec4(bufferColor.b, 1.0 - bufferColor.g, bufferColor.r, 1.0);

	const float CLIP_DISTANCE = 1000.0;
	float fragDist = pow(texture(uOutsideDepthBuffer, fs_in.uv).r, 16.0) * 1.50 - 0.5;


	outColor = mix(texture(uOutsideBuffer, fs_in.uv), vec4(0.4, 0.5, 1.0, 1.0), fragDist);// + average / 16.0;//273.0;
}