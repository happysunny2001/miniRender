#ifndef __COMPUTE_TILES_HLSL__
#define __COMPUTE_TILES_HLSL__

#include "TBDRDefines.hlsl"
#include "Light.hlsl"
#include "GBuffer.hlsl"
#include "GeometryTest.hlsl"

RWTexture2D<float4> gLitTexture;

groupshared uint sTileLightIndeces[MAX_LIGHTS];
groupshared uint sTileLightCount;

groupshared uint sMinZ;
groupshared uint sMaxZ;

[numthreads(COMPUTE_SHADER_TILE_GROUP_DIM, COMPUTE_SHADER_TILE_GROUP_DIM, 1)]
void TBDRCS(uint3 groupId          : SV_GroupID,
            uint3 dispatchThreadId : SV_DispatchThreadID,
            uint3 groupThreadId    : SV_GroupThreadID)
{
	uint groupIndex = groupThreadId.y * COMPUTE_SHADER_TILE_GROUP_DIM + groupThreadId.x;

	uint2 globalCoords = dispatchThreadId.xy;
	SurfaceData sData = ComputeSurfaceFromGBuffer(globalCoords);

	//gCameraNearFarZ, x:Far Plane Z value, y:Near Plane Z value
	float fMinSampleZ = gCameraNearFarZ.x;
	float fMaxSampleZ = gCameraNearFarZ.y;
	float fViewSpaceZ = sData.positionView.z;
	bool bValidPixel = fViewSpaceZ >= gCameraNearFarZ.y && fViewSpaceZ < gCameraNearFarZ.x;

	[flatten] if(bValidPixel) {
		fMinSampleZ = min(fMinSampleZ, fViewSpaceZ);
		fMaxSampleZ = max(fMaxSampleZ, fViewSpaceZ);
	}

	if(groupIndex == 0) {
		sTileLightCount = 0;
		sMinZ = 0x7F7FFFFF;
		sMaxZ = 0;
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

	for(uint lightIndex = groupIndex; lightIndex < gPointLightCnt; lightIndex += COMPUTE_SHADER_TILE_GROUP_DIM) {
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
		float3 lit = float3(0.0f, 0.0f, 0.0f);
		for(uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex) {
			PointLight light = gPointLights[sTileLightIndeces[tileLightIndex]];
			PointLightAccumulate(light, sData, lit);
		}

		//float f = numLights * rcp(gPointLightCnt);
		//gLitTexture[globalCoords.xy] = float4(f, f, f, 1.0f);
		gLitTexture[globalCoords.xy] = float4(lit, 1.0f);
	}
}

#endif
