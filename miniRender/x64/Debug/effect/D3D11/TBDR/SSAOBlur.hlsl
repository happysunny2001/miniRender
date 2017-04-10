#ifndef __SSAO_BLUR_H__
#define __SSAO_BLUR_H__

#include "PerFrameConstant.hlsl"
#include "GBuffer.hlsl"
#include "VertexLayout.hlsl"
#include "SSAO.hlsl"

cbuffer cbSettings
{
	float gBlurWeights[11] = {
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
	};	
};

cbuffer cbConstant
{
	static const int gBlurRadius = 5;
};

SamplerState samSSAOInputImage
{
	Filter    = MIN_MAG_LINEAR_MIP_POINT;
	AddressU  = CLAMP;
    AddressV  = CLAMP;
};

Texture2D gSSAOBlurTexture;

float3 getNormalViewSpace(int2 coord, uint sampleIndex)
{
	float4 normal = gNormalTexture.Load(coord, sampleIndex);
	return normalize(DecodeSphereMap(normal.xy));
}

float getDepthInViewSpace(int2 coord, uint sampleIndex)
{
	float depth = gDepthTexture.Load(coord, sampleIndex).x;
	return gMatProj._43 / (depth - gMatProj._33);
}

VertexSSAOOutput SSAO_Blur_VS(VertexPosTex pIn)
{
	VertexSSAOOutput pOut;
	pOut.pos = float4(pIn.PosL, 1.0f);
	pOut.Tex = pIn.Tex;
	return pOut;
}

float4 SSAO_Blur_PS(VertexSSAOOutput pIn, uniform bool gHorizBlur) : SV_Target
{
	float2 dim;
	uint temp;
	gDepthTexture.GetDimensions(dim.x, dim.y, temp);

	int2 texOffset;
	float2 imageOffset;
	if(gHorizBlur) {
		texOffset = int2(1.0f, 0);
		imageOffset = float2(1.0f/dim.x, 0);
	}
	else {
		texOffset = int2(0, 1.0f);
		imageOffset = float2(0, 1.0f/dim.y);
	}

	float3 centerNormal = getNormalViewSpace(int2(pIn.pos.xy), 0);
	float centerDepth = getDepthInViewSpace(int2(pIn.pos.xy), 0);
	float totalWeight = 0.0f;
	float4 color = 0.0f;

	[unroll]
	for(int i = -gBlurRadius; i <= gBlurRadius; ++i) {
		int2 currOffset = i*texOffset;
		int2 nPos = int2(pIn.pos.xy) + currOffset;

		float3 nNormal = getNormalViewSpace(nPos, 0);
		float nDepth = getDepthInViewSpace(nPos, 0);

		float weight = gBlurWeights[i+gBlurRadius];
		totalWeight += weight;

		float2 nImagePos = pIn.Tex + i*imageOffset;
		color += weight*gSSAOBlurTexture.SampleLevel(samSSAOInputImage, nImagePos, 0);
	}

	return color/totalWeight;
}

#endif
