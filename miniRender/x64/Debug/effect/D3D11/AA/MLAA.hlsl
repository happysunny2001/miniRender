#ifndef __AA_MLAA_HLSL__
#define __AA_MLAA_HLSL__

#include "TBDR/PerFrameConstant.hlsl"

#define NUM_DISTANCES 32
#define AREA_SIZE (NUM_DISTANCES*5)

cbuffer MLAA_PreFrame_Constant
{
	float fMaxSearchSteps;
	float fDepthThreshold;
	float2 f2PixelSize;
	float4x4 gMatProjDefault;
}

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

Texture2D gTexAreaBuffer;
Texture2D gTexDepthBuffer;
Texture2D gTexEdgesBuffer;
Texture2D gTexBlendBuffer;
Texture2D gTexColorBuffer;
Texture2D gTexTestBuffer;

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

VertexOut MLAAVS(VertexIn vIn)
{
	VertexOut vOut;

	float4x4 matWorldViewProj = mul(gMatWorld, gMatViewProj);
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), matWorldViewProj);
	vOut.Tex  = vIn.Tex;
    
	return vOut;
}

float4 MLAAPS(VertexOut pIn) : SV_Target
{
	return gTexTestBuffer.SampleLevel(LinearSampler, pIn.Tex, 0);
	//return gTexEdgesBuffer.SampleLevel(LinearSampler, pIn.Tex, 0);
}

float4 EdgeDetectionPS(VertexOut pIn) : SV_Target
{
	float d       = gMatProjDefault._43/(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0).x - gMatProjDefault._33);
	float dLeft   = gMatProjDefault._43/(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0, -int2(1, 0)).x - gMatProjDefault._33);
	float dTop    = gMatProjDefault._43/(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0, -int2(0, 1)).x - gMatProjDefault._33);
	float dRight  = d;//gMatProjDefault._43/(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0,  int2(1, 0)).x - gMatProjDefault._33);
	float dBottom = d;//gMatProjDefault._43/(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0,  int2(0, 1)).x - gMatProjDefault._33);

	float4 delta = abs(d.xxxx - float4(dLeft, dTop, dRight, dBottom));
	float4 edges = step(fDepthThreshold.xxxx, delta);

	if(dot(edges, 1.0) == 0)
		discard;

	return edges;
	//return float4(1.0, 1.0, 1.0, 1.0);
}

float4 EdgeDetectionColorPS(VertexOut pIn) : SV_Target
{
	float3 weights = float3(0.2126,0.7152, 0.0722);

	float L        = dot(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0).rgb, weights);
    float Lleft    = dot(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0, -int2(1, 0)).rgb, weights);
    float Ltop     = dot(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0, -int2(0, 1)).rgb, weights);
    float Lright   = dot(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0, int2(1, 0)).rgb, weights);
    float Lbottom  = dot(gTexDepthBuffer.SampleLevel(PointSampler, pIn.Tex, 0, int2(0, 1)).rgb, weights);

    float4 delta = abs(L.xxxx - float4(Lleft, Ltop, Lright, Lbottom));
    float4 edges = step(fDepthThreshold.xxxx, delta);

    if (dot(edges, 1.0) == 0.0) {
        discard;
    }

    return edges;
}

float SearchXLeft(float2 texCoord)
{
	texCoord -= float2(1.5f, 0)*f2PixelSize;
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.SampleLevel(LinearSampler, texCoord, 0).g;

		[flatten] if(e < 0.9f) break;
		texCoord -= float2(2.0f, 0)*f2PixelSize;
	}

	return max(-2.0f*i-2.0*e, -2.0f*fMaxSearchSteps);
}

float SearchXRight(float2 texCoord)
{
	texCoord += float2(1.5f, 0)*f2PixelSize;
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.SampleLevel(LinearSampler, texCoord, 0).g;

		[flatten] if(e < 0.9f) break;
		texCoord += float2(2.0f, 0)*f2PixelSize;
	}

	return min(2.0f*i+2.0*e, 2.0f*fMaxSearchSteps);
}

float SearchYUp(float2 texCoord)
{
	texCoord -= float2(0, 1.5f)*f2PixelSize;
	float e = 0.0f;

	for(int i = 0; i < fMaxSearchSteps; ++i) {
		e = gTexEdgesBuffer.SampleLevel(LinearSampler, texCoord, 0).r;

		[flatten] if(e < 0.9f) break;
		texCoord -= float2(0, 2.0f)*f2PixelSize;
	}

	return max(-2.0f*i-2.0*e, -2.0f*fMaxSearchSteps);
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

float2 Area(float2 distance, float e1, float e2)
{
	float2 pixCoord = NUM_DISTANCES * round(4.0f * float2(e1, e2)) + distance;
	float2 texCoord = pixCoord / (AREA_SIZE - 1.0f);
	return gTexAreaBuffer.SampleLevel(PointSampler, texCoord, 0).rg;
}

float4 BlendingWeightCalculationPS(VertexOut pIn) : SV_Target
{
	float4 weights = 0.0f;
	float2 e = gTexEdgesBuffer.SampleLevel(PointSampler, pIn.Tex, 0).rg;

	[branch]
	if(e.g) {
		float2 d = float2(SearchXLeft(pIn.Tex), SearchXRight(pIn.Tex));
		float4 coords = mad(float4(d.x, -0.25f, d.y+1.0f, -0.25f), f2PixelSize.xyxy, pIn.Tex.xyxy);
		float e1 = gTexEdgesBuffer.SampleLevel(LinearSampler, coords.xy, 0).r;
		float e2 = gTexEdgesBuffer.SampleLevel(LinearSampler, coords.zw, 0).r;
		weights.rg = Area(abs(d), e1, e2);
	}

	[branch]
	if(e.r) {
		float2 d = float2(SearchYUp(pIn.Tex), SearchYDown(pIn.Tex));
		float4 coords = mad(float4(-0.25f, d.x, -0.25f, d.y+1.0f), f2PixelSize.xyxy, pIn.Tex.xyxy);
		float e1 = gTexEdgesBuffer.SampleLevel(LinearSampler, coords.xy, 0).g;
		float e2 = gTexEdgesBuffer.SampleLevel(LinearSampler, coords.zw, 0).g;
		weights.ba = Area(abs(d), e1, e2);
	}

	return weights;
}

float4 NeighbourBlendingPS(VertexOut pIn) : SV_Target
{
	float4 topLeft = gTexBlendBuffer.SampleLevel(PointSampler, pIn.Tex, 0);
	float right    = gTexBlendBuffer.SampleLevel(PointSampler, pIn.Tex, 0, int2(0, 1)).g;
	float bottom   = gTexBlendBuffer.SampleLevel(PointSampler, pIn.Tex, 0, int2(1, 0)).a;
	float4 a = float4(topLeft.r, right, topLeft.b, bottom);

	float sum = dot(a, 1.0f);

	[branch]
	if(sum > 0) {
		float4 o = a*f2PixelSize.yyxx;
		float4 color = 0;
		color = mad(gTexColorBuffer.SampleLevel(LinearSampler, pIn.Tex+float2( 0.0f, -o.r ), 0), a.r, color);
		color = mad(gTexColorBuffer.SampleLevel(LinearSampler, pIn.Tex+float2( 0.0f,  o.g ), 0), a.g, color);
		color = mad(gTexColorBuffer.SampleLevel(LinearSampler, pIn.Tex+float2(-o.b,   0.0f), 0), a.b, color);
		color = mad(gTexColorBuffer.SampleLevel(LinearSampler, pIn.Tex+float2( o.a,   0.0f), 0), a.a, color);
		return color / sum;
		//return float4(1.0f, 0, 0, 1.0f);
	}
	else {
		return gTexColorBuffer.SampleLevel(LinearSampler, pIn.Tex, 0);
	}
}

DepthStencilState DisableDepthStencil {
    DepthEnable = FALSE;
    StencilEnable = FALSE;
};

DepthStencilState DisableDepthReplaceStencil {
    DepthEnable = FALSE;
    StencilEnable = TRUE;
    FrontFaceStencilPass = REPLACE;
};

DepthStencilState DisableDepthUseStencil {
    DepthEnable = FALSE;
    StencilEnable = TRUE;
    FrontFaceStencilFunc = EQUAL;
};

BlendState NoBlending {
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};

technique11 MLAAEdgeDetection
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, MLAAVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, EdgeDetectionPS() ) );

        SetDepthStencilState(DisableDepthReplaceStencil, 1);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 MLAAEdgeDetectionColor
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, MLAAVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, EdgeDetectionColorPS() ) );

        //SetDepthStencilState(DisableDepthReplaceStencil, 1);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 MLAABlendingWeight
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, MLAAVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, BlendingWeightCalculationPS() ) );

        SetDepthStencilState(DisableDepthUseStencil, 1);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 MLAANeighbourBlending
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, MLAAVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, NeighbourBlendingPS() ) );

        //SetDepthStencilState(DisableDepthUseStencil, 1);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 MLAATest
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, MLAAVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, MLAAPS() ) );
	}
}

#endif
