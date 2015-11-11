#include "util.fx"
#include "lightUtil.fx"

Texture2D gWaveTexture0;
Texture2D gWaveTexture1;

cbuffer cbPerFrame
{
	float gHeightScale0;
	float gHeightScale1;
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMaxTessFactor;
	float gMinTessFactor;
};

cbuffer cbPerObject
{
	float4x4 gWavevDispTexTransform0;
	float4x4 gWavevDispTexTransform1;
	float4x4 gWavevNormalTexTransform0;
	float4x4 gWavevNormalTexTransform1;
};

struct VertexInPNTT
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float2 Tex      : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOutPNTT
{
    float3 PosW      : POSITION;    //vertex position in world space
    float3 NormalW   : NORMAL;      //vertex normal in world space
    float3 TangentW  : TANGENT;     //vertex tangent in world space
    float2 Tex       : TEXCOORD0;    //pixel texture coord
    float2 WaveDispTex0 : TEXCOORD1;  
    float2 WaveDispTex1 : TEXCOORD2;
    float2 WaveNormalTex0 : TEXCOORD3;  
    float2 WaveNormalTex1 : TEXCOORD4;
    float TessFactor : TESS;        //tessellation factor base on vertex and view position in world space
};

VertexOutPNTT VS(VertexInPNTT vIn)
{
	VertexOutPNTT vOut;

	vOut.PosW     = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.NormalW  = mul(vIn.NormalL, (float3x3)gMatWorldInvTranspose).xyz;
	vOut.TangentW = mul(vIn.TangentL, (float3x3)gMatWorld).xyz;
	vOut.Tex      = vIn.Tex;
	vOut.WaveDispTex0   = mul(float4(vIn.Tex, 0.0f, 1.0f), gWavevDispTexTransform0).xy;
	vOut.WaveDispTex1   = mul(float4(vIn.Tex, 0.0f, 1.0f), gWavevDispTexTransform1).xy;
	vOut.WaveNormalTex0 = mul(float4(vIn.Tex, 0.0f, 1.0f), gWavevNormalTexTransform0).xy;
	vOut.WaveNormalTex1 = mul(float4(vIn.Tex, 0.0f, 1.0f), gWavevNormalTexTransform1).xy;

	//gMinTessDistance is large than gMaxTessDistance
	float dist = distance(vOut.PosW, gEyePosWorld);
	float tess = saturate((gMinTessDistance-dist)/(gMinTessDistance-gMaxTessDistance));
	vOut.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);

	return vOut;
}

struct PatchTessTriangle
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess    : SV_InsideTessFactor;
};

PatchTessTriangle ConstantHS(InputPatch<VertexOutPNTT, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTessTriangle patchOut;

	patchOut.EdgeTess[0] = 0.5f*(patch[1].TessFactor + patch[2].TessFactor);
	patchOut.EdgeTess[1] = 0.5f*(patch[2].TessFactor + patch[0].TessFactor);
	patchOut.EdgeTess[2] = 0.5f*(patch[0].TessFactor + patch[1].TessFactor);

	patchOut.InsideTess = patchOut.EdgeTess[0];

	return patchOut;
}

struct HullOut
{
	float3 PosW     : POSITION;
	float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex      : TEXCOORD0;
	float2 WaveDispTex0   : TEXCOORD1;  
	float2 WaveDispTex1   : TEXCOORD2;
	float2 WaveNormalTex0 : TEXCOORD3;  
	float2 WaveNormalTex1 : TEXCOORD4;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
HullOut HS(InputPatch<VertexOutPNTT, 3> p, 
		   uint i : SV_OutputControlPointID,
		   uint patchId : SV_PrimitiveID)
{
	HullOut hOut;
	hOut.PosW     = p[i].PosW;
	hOut.NormalW  = p[i].NormalW;
	hOut.TangentW = p[i].TangentW;
	hOut.Tex      = p[i].Tex;
	hOut.WaveDispTex0 = p[i].WaveDispTex0;
	hOut.WaveDispTex1 = p[i].WaveDispTex1;
	hOut.WaveNormalTex0 = p[i].WaveNormalTex0;
	hOut.WaveNormalTex1 = p[i].WaveNormalTex1;
	return hOut;
}

struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
	float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex      : TEXCOORD0;
	float2 WaveDispTex0   : TEXCOORD1;  
	float2 WaveDispTex1   : TEXCOORD2;
	float2 WaveNormalTex0 : TEXCOORD3;  
	float2 WaveNormalTex1 : TEXCOORD4;
};

[domain("tri")]
DomainOut DS(PatchTessTriangle patchTess, float3 uvw : SV_DomainLocation, const OutputPatch<HullOut, 3> triData)
{
	DomainOut dOut;

	dOut.PosW           = triData[0].PosW*uvw.x           + triData[1].PosW*uvw.y           + triData[2].PosW*uvw.z;
	dOut.NormalW        = triData[0].NormalW*uvw.x        + triData[1].NormalW*uvw.y        + triData[2].NormalW*uvw.z;
	dOut.TangentW       = triData[0].TangentW*uvw.x       + triData[1].TangentW*uvw.y       + triData[2].TangentW*uvw.z;
	dOut.Tex            = triData[0].Tex*uvw.x            + triData[1].Tex*uvw.y            + triData[2].Tex*uvw.z;
	dOut.WaveDispTex0   = triData[0].WaveDispTex0*uvw.x   + triData[1].WaveDispTex0*uvw.y   + triData[2].WaveDispTex0*uvw.z;
	dOut.WaveDispTex1   = triData[0].WaveDispTex1*uvw.x   + triData[1].WaveDispTex1*uvw.y   + triData[2].WaveDispTex1*uvw.z;
	dOut.WaveNormalTex0 = triData[0].WaveNormalTex0*uvw.x + triData[1].WaveNormalTex0*uvw.y + triData[2].WaveNormalTex0*uvw.z;
	dOut.WaveNormalTex1 = triData[0].WaveNormalTex1*uvw.x + triData[1].WaveNormalTex1*uvw.y + triData[2].WaveNormalTex1*uvw.z;

	dOut.NormalW = normalize(dOut.NormalW);

	const float MipInterval = 20.0f;
	float mipLevel = clamp( (distance(dOut.PosW, gEyePosWorld) - MipInterval) / MipInterval, 0.0f, 6.0f);

	float h0 = gWaveTexture0.SampleLevel(samLinear, dOut.WaveDispTex0, mipLevel).a;
	float h1 = gWaveTexture1.SampleLevel(samLinear, dOut.WaveDispTex1, mipLevel).a;
	
	//dOut.PosW += (gHeightScale0*(h0))*dOut.NormalW;
	dOut.PosW.y += gHeightScale0*h0;
	dOut.PosW.y += gHeightScale1*h1;

	dOut.PosH = mul(float4(dOut.PosW, 1.0f), gViewProj);

	return dOut;
}

float4 PS(DomainOut pIn, uniform bool gAlphaClip, uniform bool gFogEnable) : SV_Target
{
	pIn.NormalW   = normalize(pIn.NormalW);
	float3 toEyeW = gEyePosWorld - pIn.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

	float3 normalMapSampleT0 = gWaveTexture0.Sample(samLinear, pIn.WaveNormalTex0).rgb;
	float3 bumpedNormalW0 = processNormalMapToWorld(normalMapSampleT0, pIn.NormalW, pIn.TangentW);

	float3 normalMapSampleT1 = gWaveTexture1.Sample(samLinear, pIn.WaveNormalTex1).rgb;
	float3 bumpedNormalW1 = processNormalMapToWorld(normalMapSampleT1, pIn.NormalW, pIn.TangentW);

	float3 bumpedNormalW = normalize(bumpedNormalW0+bumpedNormalW1);
	
	float4 ambient, diffuse, spec;
	processLight(gMaterial, pIn.PosW, bumpedNormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = (ambient + diffuse) + spec;

	if(gFogEnable) {
		float s = saturate((toEyeDistance-gFogStart)/gFogRange);
		litColor = lerp(litColor, gFogColor, s);
	}
	
	float4 skyColor = gSkyCubeMap.Sample(samAnisotropic, pIn.PosW-float3(0.0f, -100.0f, 0.0f));
	float4 finalColor = skyColor*0.5f + litColor*0.5f;

	finalColor.a = gMaterial.diffuse.a;

	return finalColor;
}

technique11 TechWave
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false) ) );
    }
}
