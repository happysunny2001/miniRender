#ifndef __BLENDING_WEIGHT_HLSL__
#define __BLENDING_WEIGHT_HLSL__

#define COMPUTE_SHADER_GROUP_SIZE 32

Texture2D gTexEdgesBuffer;
Texture2D gTexAreaBuffer;
StructuredBuffer<float4> gAreaBuffer;

RWTexture2D<float4> gTexBlendBuffer;

float SearchXLeft(uint2 texCoord)
{
	texCoord -= int2(1, 0);
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.Load(int3(texCoord, 0)).g;

		[flatten] if(e < 0.9f) break;
		texCoord -= int2(1, 0);
	}

	return max(-i, -fMaxSearchSteps);
}

float SearchXRight(uint2 texCoord)
{
	texCoord += int2(1, 0);
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.Load(int3(texCoord, 0)).g;

		[flatten] if(e < 0.9f) break;
		texCoord += int2(1, 0);
	}

	return min(i, fMaxSearchSteps);
}

float SearchYUp(int2 texCoord)
{
	texCoord -= int2(0, 1);
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.Load(int3(texCoord, 0)).r;

		[flatten] if(e < 0.9f) break;
		texCoord -= int2(0, 1);
	}

	return max(-i, -fMaxSearchSteps);
}

float SearchYDown(float2 texCoord)
{
	texCoord += float2(0, 1.5f)*f2PixelSize;
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.SampleLevel(LinearSampler, texCoord, 0).r;

		[flatten] if(e < 0.9f) break;
		texCoord += float2(0, 2.0f)*f2PixelSize;
	}

	return min(2.0f*i+2.0*e, 2.0f*fMaxSearchSteps);
}

[numthreads(COMPUTE_SHADER_GROUP_SIZE, COMPUTE_SHADER_GROUP_SIZE, 1)]
void MLAABlendingCS(uint3 groupId          : SV_GroupID,
            		uint3 dispatchThreadId : SV_DispatchThreadID,
            		uint3 groupThreadId    : SV_GroupThreadID)
{
	uint2 globalCoords = dispatchThreadId.xy;
	float2 e = gTexEdgesBuffer.Load(int3(globalCoords.xy, 0)).rg;

	[branch]
	if(e.g) {

	}
}

#endif
