#ifndef __SSAO_HLSL__
#define __SSAO_HLSL__

#include "PerFrameConstant.hlsl"
#include "SurfaceData.hlsl"
#include "GBuffer.hlsl"
#include "VertexLayout.hlsl"

cbuffer cbSSAO
{
	float4x4 gViewToTexSpace; // Proj*Texture
	float4 gOffsetVectors[14];

	// Coordinates given in view space.
	float    gOcclusionRadius    = 0.5f;
	float    gOcclusionFadeStart = 0.2f;
	float    gOcclusionFadeEnd   = 2.0f;
	float    gSurfaceEpsilon     = 0.05f;
};

struct SurfaceSSAOData
{
	float3 positionView;
	float3 normalView;
};

struct VertexSSAOOutput
{
	float4 pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

Texture2D gSSAORandVecMap;

SamplerState samRandomVec
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

SurfaceSSAOData ComputeSSAOSurfaceFromGBuffer(uint2 globalCoords, uint sampleIndex)
{
	SurfaceSSAOData sData;

	float depth = gDepthTexture.Load(int2(globalCoords.xy), sampleIndex).x;
	float4 normal = gNormalTexture.Load(int2(globalCoords.xy), sampleIndex);

	sData.normalView = normalize(DecodeSphereMap(normal.xy));

	float2 dim;
	uint temp;
	gDepthTexture.GetDimensions(dim.x, dim.y, temp);

	float2 screenPixelOffset = float2(2.0f, -2.0f) / dim;
    float2 positionScreen = (float2(globalCoords.xy) + 0.5f) * screenPixelOffset.xy + float2(-1.0f, 1.0f);

    float viewSpaceZ = gMatProj._43 / (depth - gMatProj._33);
    sData.positionView = ComputePositionViewFromZ(positionScreen, viewSpaceZ).xyz;

    return sData;
}

// Determines how much the sample point q occludes the point p as a function
// of distZ.
float OcclusionFunction(float distZ)
{
	//
	// If depth(q) is "behind" depth(p), then q cannot occlude p.  Moreover, if 
	// depth(q) and depth(p) are sufficiently close, then we also assume q cannot
	// occlude p because q needs to be in front of p by Epsilon to occlude p.
	//
	// We use the following function to determine the occlusion.  
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//
	
	float occlusion = 0.0f;
	if(distZ > gSurfaceEpsilon)
	{
		float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;
		
		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
		occlusion = saturate( (gOcclusionFadeEnd-distZ)/fadeLength );
	}
	
	return occlusion;	
}

VertexSSAOOutput SSAO_VS(VertexPosTex pIn)
{
	VertexSSAOOutput pOut;
	pOut.pos = float4(pIn.PosL, 1.0f);
	pOut.Tex = pIn.Tex;
	return pOut;
}

float4 SSAO_PS(VertexSSAOOutput pIn) : SV_Target
{
	SurfaceSSAOData surface = ComputeSSAOSurfaceFromGBuffer(pIn.pos.xy, 0);

	float2 dim;
	uint temp;
	gDepthTexture.GetDimensions(dim.x, dim.y, temp);
	float occlusionSum = 0.0f;

	float3 randVec = 2.0f*gSSAORandVecMap.SampleLevel(samRandomVec, 4.0f*pIn.Tex, 0).rgb - 1.0f;

	for(int i = 0; i < 14; ++i) {
		float3 offset = reflect(gOffsetVectors[i].xyz, randVec);
		
		float flip = sign(dot(surface.normalView, offset));
		float3 q = surface.positionView + flip*gOcclusionRadius*offset;

		float4 projQ = mul(float4(q, 1.0f), gViewToTexSpace);
		projQ /= projQ.w;

		float rz = gDepthTexture.Load(int2(projQ.xy*dim), 0).x;
		rz = gMatProj._43 / (rz - gMatProj._33);
		float3 r = rz/q.z * q;

		float dp = max(dot(normalize(r-surface.positionView), surface.normalView), 0.0f);

		float distZ = surface.positionView.z - r.z;
		float occlusion = dp * OcclusionFunction(distZ);

		occlusionSum += occlusion;
	}

	occlusionSum /= 14;
	float access = 1.0f - occlusionSum;

	float f = saturate(pow(access, 4.0f));
	return float4(f, f, f, 1.0f);
}

#endif