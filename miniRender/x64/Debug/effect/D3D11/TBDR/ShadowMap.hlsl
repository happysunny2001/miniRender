#ifndef __SHADOW_MAP_HLSL__
#define __SHADOW_MAP_HLSL__

//#include "PerFrameConstant.hlsl"

cbuffer c1
{
	float4x4 gMatWorld;
	float4x4 gMatShadowViewProj;
};

struct VertexShadowIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexShadowOut
{
	float4 PosH    : SV_POSITION;
};

VertexShadowOut ShadowVS(VertexShadowIn vin)
{
	VertexShadowOut vout;

	float4x4 matWorldViewProj = mul(gMatWorld, gMatShadowViewProj);
	vout.PosH  = mul(float4(vin.PosL, 1.0f), matWorldViewProj);

	return vout;
}

RasterizerState ShadowDepth
{
	// [From MSDN]
	// If the depth buffer currently bound to the output-merger stage has a UNORM format or
	// no depth buffer is bound the bias value is calculated like this: 
	//
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// where r is the minimum representable value > 0 in the depth-buffer format converted to float32.
	// [/End MSDN]
	// 
	// For a 24-bit depth buffer, r = 1 / 2^24.
	//
	// Example: DepthBias = 100000 ==> Actual DepthBias = 100000/2^24 = .006

	// You need to experiment with these values for your scene.
	DepthBias = 10000;
	DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 1.0f;
};

technique11 BuildShadowMap
{
	pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, ShadowVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( NULL );
		SetRasterizerState(ShadowDepth);
    }
}

#endif