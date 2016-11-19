#ifndef __COMPUTE_TILES_HLSL__
#define __COMPUTE_TILES_HLSL__

#include "TBDRDefines.hlsl"
#include "Light.hlsl"
#include "GBuffer.hlsl"
#include "GeometryTest.hlsl"
#include "FrameBuffer.hlsl"

RWStructuredBuffer<float4> gLitFrameBuffer;

groupshared uint sTileLightIndeces[MAX_LIGHTS];
groupshared uint sTileLightCount;

groupshared uint sPerSamplePixel[COMPUTE_SHADER_TILE_GROUP_SIZE];
groupshared uint sPreSamplePixelCount;

groupshared uint sMinZ;
groupshared uint sMaxZ;

void WriteSample(uint2 globalCoords, uint sampleIndex, float4 value)
{
	gLitFrameBuffer[GetFrameBufferAddress(globalCoords, sampleIndex)] = value;
}

uint packCoords(uint2 coords) 
{
	return coords.y<<16|(0x0000FFFF&coords.x);
}

uint2 unpackCoords(uint coord) 
{
	return uint2(coord&0xFFFF, coord>>16);
}

bool IsRequestPerSampleShading(SurfaceData surfaceSamples[MSAA_SAMPLES])
{
	bool bComplexSample = false;
	float sample0Depth = surfaceSamples[0].positionView.z;
	float3 sample0Normal = surfaceSamples[0].normalView;

	[unroll]
	for(uint iSample = 1; iSample < MSAA_SAMPLES; ++iSample) {
		float sampleIDepth = surfaceSamples[iSample].positionView.z;
		float3 sampleINormal = surfaceSamples[iSample].normalView;

		[branch]
		if(abs(sampleIDepth-sample0Depth) > 1.0f || abs(dot(abs(sample0Normal-sampleINormal), float3(1,1,1))) > 0.1f) {
			bComplexSample = true;
			break;
		}
	}

	return bComplexSample;
}

[numthreads(COMPUTE_SHADER_TILE_GROUP_DIM, COMPUTE_SHADER_TILE_GROUP_DIM, 1)]
void TBDRCS(uint3 groupId          : SV_GroupID,
            uint3 dispatchThreadId : SV_DispatchThreadID,
            uint3 groupThreadId    : SV_GroupThreadID)
{
	uint groupIndex = groupThreadId.y * COMPUTE_SHADER_TILE_GROUP_DIM + groupThreadId.x;

	uint2 globalCoords = dispatchThreadId.xy;

	SurfaceData surfaceSamples[MSAA_SAMPLES];
	ComputeSurfaceFromGBufferAllSamples(globalCoords, surfaceSamples);

	//gCameraNearFarZ, x:Far Plane Z value, y:Near Plane Z value
	float fMinSampleZ = gCameraNearFarZ.x;
	float fMaxSampleZ = gCameraNearFarZ.y;

	{
		[unroll]
		for(uint iSample = 0; iSample < MSAA_SAMPLES; ++iSample) {
			float fViewSpaceZ = surfaceSamples[iSample].positionView.z;
			bool bValidPixel = fViewSpaceZ >= gCameraNearFarZ.y && fViewSpaceZ < gCameraNearFarZ.x;
			[flatten] if(bValidPixel) {
				fMinSampleZ = min(fMinSampleZ, fViewSpaceZ);
				fMaxSampleZ = max(fMaxSampleZ, fViewSpaceZ);
			}
		} 
	}

	if(groupIndex == 0) {
		sTileLightCount = 0;
		sMinZ = 0x7F7FFFFF;
		sMaxZ = 0;
		sPreSamplePixelCount = 0;
	}

	GroupMemoryBarrierWithGroupSync();

	if(fMaxSampleZ >= fMinSampleZ) {
		InterlockedMin(sMinZ, asuint(fMinSampleZ));
		InterlockedMax(sMaxZ, asuint(fMaxSampleZ));
	}

	GroupMemoryBarrierWithGroupSync();

	float fTileMinZ = asfloat(sMinZ);
	float fTileMaxZ = asfloat(sMaxZ);

	float2 tileScale = float2(gFrameBufferDimensions.xy) * rcp(float(2 * COMPUTE_SHADER_TILE_GROUP_DIM));
    float2 tileBias = tileScale - float2(groupId.xy);

    float4 c1 = float4(gMatProj._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
    float4 c2 = float4(0.0f, -gMatProj._22 * tileScale.y, tileBias.y, 0.0f);
    float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

    float4 frustumPlanes[6];
    // Sides
    frustumPlanes[0] = c4 - c1;
    frustumPlanes[1] = c4 + c1;
    frustumPlanes[2] = c4 - c2;
    frustumPlanes[3] = c4 + c2;
    // Near/far
    frustumPlanes[4] = float4(0.0f, 0.0f,  1.0f, -fTileMinZ);
    frustumPlanes[5] = float4(0.0f, 0.0f, -1.0f,  fTileMaxZ);

    // Normalize frustum planes (near/far already normalized)
    [unroll] for (uint i = 0; i < 4; ++i) {
        frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
    }

	for(uint lightIndex = groupIndex; lightIndex < gPointLightCnt; lightIndex += COMPUTE_SHADER_TILE_GROUP_SIZE) {
		PointLight light = gPointLights[lightIndex];

		bool inFrustum = true;
        [unroll] for (uint i = 0; i < 6; ++i) {
            float d = dot(frustumPlanes[i], float4(light.position.xyz, 1.0f));
            inFrustum = inFrustum && (d >= -light.radiusEnd);
        }

		[branch] if(inFrustum) {
			uint lightListIndex;
			InterlockedAdd(sTileLightCount, 1, lightListIndex);
			sTileLightIndeces[lightListIndex] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();

	uint numLights = sTileLightCount;

	if(all(globalCoords < gFrameBufferDimensions.xy)) {
		float3 lightDiffuse = float3(0, 0, 0);
		[flatten]
		if(gEnableDirectionalLight == 1) {
			DirectionalLightAccumulate(gDirectionalLight, surfaceSamples[0], lightDiffuse);
		}

		bool bComplex = IsRequestPerSampleShading(surfaceSamples);
		float3 lightFinal = float3(0, 0, 0);

		if(numLights > 0) {
			float3 lit = float3(0.0f, 0.0f, 0.0f);
			for(uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex) {
				PointLight light = gPointLights[sTileLightIndeces[tileLightIndex]];
				PointLightAccumulate(light, surfaceSamples[0], lit);
			}

			lightFinal = lit+lightDiffuse;
		}
		else {
			lightFinal = lightDiffuse;
		}

		WriteSample(globalCoords, 0, float4(lightFinal, 1.0f));

		if(bComplex) {
			uint perSampleIndex;
			InterlockedAdd(sPreSamplePixelCount, 1, perSampleIndex);
			sPerSamplePixel[perSampleIndex] = packCoords(globalCoords);
		}
		else {
			[unroll]
			for(uint iSample = 1; iSample < MSAA_SAMPLES; ++iSample) {
				WriteSample(globalCoords, iSample, float4(lightFinal, 1.0f));
			}
		}
	}

	GroupMemoryBarrierWithGroupSync();

	const uint cSampleCountPerPixel = MSAA_SAMPLES-1;
	uint uMaxSampleCount = sPreSamplePixelCount*cSampleCountPerPixel;
	for(uint iSample = groupIndex; iSample < uMaxSampleCount; iSample += COMPUTE_SHADER_TILE_GROUP_SIZE) {
		uint uPixelIndex = iSample / cSampleCountPerPixel;
		uint uSampleIndex = iSample % cSampleCountPerPixel + 1;

		uint2 sampleCoord = unpackCoords(sPerSamplePixel[uPixelIndex]);
		SurfaceData surface = ComputeSurfaceFromGBuffer(sampleCoord, uSampleIndex);

		float3 litSample = float3(0.0f, 0.0f, 0.0f);
		for(uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex) {
			PointLight light = gPointLights[sTileLightIndeces[tileLightIndex]];
			PointLightAccumulate(light, surface, litSample);
		}

		[flatten]
		if(gEnableDirectionalLight == 1) {
			DirectionalLightAccumulate(gDirectionalLight, surface, litSample);
		}

		WriteSample(sampleCoord, uSampleIndex, float4(litSample, 1.0f));
	}
}

#endif
