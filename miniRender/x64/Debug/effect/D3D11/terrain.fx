#include "util.fx"
#include "lightUtil.fx"

SamplerState samHeightMap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

cbuffer cbPerFrame
{
	// When distance is minimum, the tessellation is maximum.
	// When distance is maximum, the tessellation is minimum.
	float gMinDist;
	float gMaxDist;

	// Exponents for power of 2 tessellation.  The tessellation
	// range is [2^(gMinTess), 2^(gMaxTess)].  Since the maximum
	// tessellation is 64, this means gMaxTess can be at most 6
	// since 2^6 = 64.
	float gMinTess;
	float gMaxTess;

	float gTexelCellSpaceU;
	float gTexelCellSpaceV;
	float gWorldCellSpace;

	float2 gBoundY;
	float2 gTexScale = 50.0f;

	float4 gWorldFrustumPlanes[6];
};

Texture2D gHeightMap;
Texture2D gTextureGrass;
Texture2D gTextureDarkDirt;
Texture2D gTextureStone;
Texture2D gTextureLightDirt;
Texture2D gTextureSnow;
Texture2D gTextureBlend;

float CalcTessFactor(float3 p)
{
	float d = distance(p, gEyePosWorld);
	float s = saturate((d-gMinDist)/(gMaxDist-gMinDist));
	return pow(2, (lerp(gMaxTess, gMinTess, s)));
}

//////////////////////////////////////////vertex shader///////////////////////////////////////////////

struct VertexIn
{
	float3 PosL   : POSITION;
	float2 Tex    : TEXCOORD;
};

struct VertexOut
{
	float3 PosW   : POSITION;
	float2 Tex    : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;

	float3 posL;
	posL = vIn.PosL;
	posL.y = gHeightMap.SampleLevel(samHeightMap, vIn.Tex, 0).r;
	vOut.PosW = mul(float4(posL, 1.0f), gMatWorld).xyz;
	vOut.Tex = vIn.Tex;

	return vOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////Hull Shader///////////////////////////////////////////////////////////////////////////

struct PatchTessQuad
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTessQuad ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTessQuad ptQuad;

	float3 vMin = float3(patch[2].PosW.x, gBoundY.x, patch[2].PosW.z);
	float3 vMax = float3(patch[1].PosW.x, gBoundY.y, patch[1].PosW.z);

	float3 center = 0.5f*(vMin + vMax);
	float3 extents = 0.5f*(vMax - vMin);
	if(AabbOutsideFrustum(center, extents, gWorldFrustumPlanes)) {
		ptQuad.EdgeTess[0] = 0.0f;
		ptQuad.EdgeTess[1] = 0.0f;
		ptQuad.EdgeTess[2] = 0.0f;
		ptQuad.EdgeTess[3] = 0.0f;
		ptQuad.InsideTess[0] = 0.0f;
		ptQuad.InsideTess[1] = 0.0f;

		return ptQuad;
	}
	else {
		float3 e0 = 0.5f*(patch[0].PosW + patch[2].PosW);
		float3 e1 = 0.5f*(patch[0].PosW + patch[1].PosW);
		float3 e2 = 0.5f*(patch[1].PosW + patch[3].PosW);
		float3 e3 = 0.5f*(patch[2].PosW + patch[3].PosW);
		float3 c = 0.25f*(patch[0].PosW + patch[1].PosW + patch[2].PosW + patch[3].PosW);

		ptQuad.EdgeTess[0] = CalcTessFactor(e0);
		ptQuad.EdgeTess[1] = CalcTessFactor(e1);
		ptQuad.EdgeTess[2] = CalcTessFactor(e2);
		ptQuad.EdgeTess[3] = CalcTessFactor(e3);
		ptQuad.InsideTess[0] = CalcTessFactor(c);
		ptQuad.InsideTess[1] = ptQuad.InsideTess[0];

		return ptQuad;
	}
}

struct HullOut
{
	float3 PosW : POSITION;
	float2 Tex  : TEXCOORD;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p, 
		   uint i : SV_OutputControlPointID,
		   uint patchId : SV_PrimitiveID)
{
	HullOut hOut;
	hOut.PosW     = p[i].PosW;
	hOut.Tex      = p[i].Tex;
	return hOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////Domain Shader////////////////////////////////////////////////////

struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 TiledTex : TEXCOORD1;
};

[domain("quad")]
DomainOut DS(PatchTessQuad patch, float2 uv : SV_DomainLocation, const OutputPatch<HullOut, 4> quadData)
{
	DomainOut dOut;

	dOut.PosW = lerp(lerp(quadData[0].PosW, quadData[1].PosW, uv.x),
					 lerp(quadData[2].PosW, quadData[3].PosW, uv.x),
					 uv.y
					);
	dOut.Tex = lerp(lerp(quadData[0].Tex, quadData[1].Tex, uv.x),
					lerp(quadData[2].Tex, quadData[3].Tex, uv.x),
					uv.y);
	dOut.TiledTex = dOut.Tex*gTexScale;
	dOut.PosW.y = gHeightMap.SampleLevel(samHeightMap, dOut.Tex, 0).r;
	dOut.PosH = mul(float4(dOut.PosW, 1.0f), gViewProj);

	return dOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////Pixel Shader////////////////////////////////////////////////////

float4 PS(DomainOut pIn, uniform bool gFogEnable) : SV_Target
{
	float2 leftTex   = pIn.Tex + float2(-gTexelCellSpaceU, 0.0f);
	float2 rightTex  = pIn.Tex + float2( gTexelCellSpaceU, 0.0f);
	float2 topTex    = pIn.Tex + float2( 0.0f,            -gTexelCellSpaceV);
	float2 bottomTex = pIn.Tex + float2( 0.0f,             gTexelCellSpaceV);

	float leftY   = gHeightMap.SampleLevel(samHeightMap, leftTex, 0).r;
	float rightY  = gHeightMap.SampleLevel(samHeightMap, rightTex, 0).r;
	float topY    = gHeightMap.SampleLevel(samHeightMap, topTex, 0).r;
	float bottomY = gHeightMap.SampleLevel(samHeightMap, bottomTex, 0).r;

	float3 tangent = normalize(float3(2.0f*gWorldCellSpace, rightY-leftY, 0.0f));
	float3 bitan   = normalize(float3(0.0f, bottomY-topY, -2.0f*gWorldCellSpace));
	float3 normalW = cross(tangent, bitan);

	float3 toEyeW = gEyePosWorld - pIn.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

	float4 ambient, diffuse, spec;
	processLight(gMaterial, pIn.PosW, normalW, toEyeW, ambient, diffuse, spec);

	float4 texGrass     = gTextureGrass.Sample(samLinear, pIn.TiledTex);
	float4 texDarkDirt  = gTextureDarkDirt.Sample(samLinear, pIn.TiledTex);
	float4 texStone     = gTextureStone.Sample(samLinear, pIn.TiledTex);
	float4 texLightDirt = gTextureLightDirt.Sample(samLinear, pIn.TiledTex);
	float4 texSnow      = gTextureSnow.Sample(samLinear, pIn.TiledTex);

	float4 texBlend = gTextureBlend.Sample(samLinear, pIn.Tex);

	float4 texColor = texGrass;
	texColor = lerp(texColor, texDarkDirt, texBlend.r);
	texColor = lerp(texColor, texStone, texBlend.g);
	texColor = lerp(texColor, texLightDirt, texBlend.b);
	texColor = lerp(texColor, texSnow, texBlend.a);

	float4 litColor = texColor*(ambient + diffuse) + spec;

	if(gFogEnable) {
		float s = saturate((toEyeDistance-gFogStart)/gFogRange);
		litColor = lerp(litColor, gFogColor, s);
	}

	return litColor;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

technique11 TechTerrain
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false) ) );
    }
}
