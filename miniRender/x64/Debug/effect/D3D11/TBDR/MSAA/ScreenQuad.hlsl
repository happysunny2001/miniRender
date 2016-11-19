#ifndef __SCREEN_QUAD_HLSL__
#define __SCREEN_QUAD_HLSL__

#include "PerFrameConstant.hlsl"
#include "TBDRDefine.hlsl"
#include "FrameBuffer.hlsl"

StructuredBuffer<float4> gLitResultFrameBuffer;

//for test
Texture2D gQuadTexture;

SamplerState samLinear
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

struct VertexInTest
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexOutTest
{
	float4 PosH    : SV_POSITION; //vertex position in homogeneous space
	float3 NormalV : NORMAL;      //vertex normal in view space
};

VertexOut ScreenQuadVS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	float4x4 matWorldViewProj = mul(gMatWorld, gMatViewProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), matWorldViewProj);
	vout.Tex  = vin.Tex;
    
	return vout;
}

VertexOutTest TestVS(VertexInTest vIn)
{
	VertexOutTest vOut;

	float4x4 matWorldViewProj = mul(gMatWorld, gMatViewProj);
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), matWorldViewProj);
	vOut.NormalV = mul(mul(float4(vIn.NormalL, 0.0f), gMatWorldInvTranspose), gMatView).xyz;

	return vOut;
}

float4 ScreenQuadPS(VertexOut pin) : SV_Target
{
	uint2 coord = uint2(pin.Tex.x*gFrameBufferDimensions.x, pin.Tex.y*gFrameBufferDimensions.y);

	float3 lit = float3(0, 0, 0);
	for(uint iSample = 0; iSample < MSAA_SAMPLES; ++iSample) {
		float3 sampleLit = gLitResultFrameBuffer[GetFrameBufferAddress(coord, iSample)].xyz;

		lit += sampleLit;
	}

	return float4(lit*rcp(MSAA_SAMPLES), 1.0f);
}

float4 ScreenQuadRGPS(VertexOut pin) : SV_Target
{
	return float4(gQuadTexture.Sample( samLinear, pin.Tex ).rg, 0.0f, 1.0f);
}

float4 ScreenQuadGrayPS(VertexOut pin) : SV_Target
{
	return float4(gQuadTexture.Sample( samLinear, pin.Tex ).rrr, 1.0f);
}

float4 TestPS(VertexOutTest pin) : SV_Target
{
	return float4(0.5f, 1.0f, 1.0f, 1.0f);
}

#endif
