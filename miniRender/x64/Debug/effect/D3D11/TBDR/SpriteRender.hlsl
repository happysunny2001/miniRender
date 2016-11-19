#ifndef __SPRITE_RENDER_HLSL__
#define __SPRITE_RENDER_HLSL__

#include "PerFrameConstant.hlsl"

SamplerState samResultLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float2 Tex   : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

Texture2D gSpriteTexture;

VertexOut SpriteRenderVS(VertexIn vin)
{
	VertexOut vOut;
	
	// Transform to homogeneous clip space.
	float4x4 matWorldViewProj = mul(gMatWorld, gMatViewProj);
	vOut.PosH = mul(float4(vin.PosL, 1.0f), matWorldViewProj);
	vOut.Tex  = vin.Tex;
    
	return vOut;
}

float4 SpriteRenderPS(VertexOut pIn) : SV_Target
{
	return gSpriteTexture.Sample(samResultLinear, pIn.Tex);
}

float4 SpriteRenderGrayPS(VertexOut pIn) : SV_Target
{
	float4 color = gSpriteTexture.Sample( samResultLinear, pIn.Tex ).r;
	return float4(color.rrr, 1);
}

#endif