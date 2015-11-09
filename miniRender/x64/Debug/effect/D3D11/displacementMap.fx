#include "util.fx"
#include "lightUtil.fx"

cbuffer cbPerFrame
{
	float gHeightScale;
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMaxTessFactor;
	float gMinTessFactor;
};

//////////////////////////////////////////vertex shader///////////////////////////////////////////////

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
    float2 Tex       : TEXCOORD;    //pixel texture coord
    float TessFactor : TESS;        //tessellation factor base on vertex and view position in world space
};

VertexOutPNTT VS(VertexInPNTT vIn)
{
	VertexOutPNTT vOut;

	vOut.PosW     = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.NormalW  = mul(vIn.NormalL, (float3x3)gMatWorldInvTranspose).xyz;
	vOut.TangentW = mul(vIn.TangentL, (float3x3)gMatWorld).xyz;
	vOut.Tex      = vIn.Tex;//mul(float4(vIn.Tex, 0.0f, 1.0f), gDiffTexTransform).xy;

	//gMinTessDistance is large than gMaxTessDistance
	float dist = distance(vOut.PosW, gEyePosWorld);
	float tess = saturate((gMinTessDistance-dist)/(gMinTessDistance-gMaxTessDistance));
	vOut.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);

	return vOut;
}

VertexOutPNTT VSWave(VertexInPNTT vIn)
{
	VertexOutPNTT vOut;

	vOut.PosW     = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.NormalW  = mul(vIn.NormalL, (float3x3)gMatWorldInvTranspose).xyz;
	vOut.TangentW = mul(vIn.TangentL, (float3x3)gMatWorld).xyz;
	vOut.Tex      = mul(float4(vIn.Tex, 0.0f, 1.0f), gDiffTexTransform).xy;

	//gMinTessDistance is large than gMaxTessDistance
	float dist = distance(vOut.PosW, gEyePosWorld);
	float tess = saturate((gMinTessDistance-dist)/(gMinTessDistance-gMaxTessDistance));
	vOut.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);

	return vOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////Constant Hull Shader//////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////Hull Shader////////////////////////////////////////////////
struct HullOut
{
	float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex      : TEXCOORD;
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
	return hOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////Domain Shader////////////////////////////////////////////////////
struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex      : TEXCOORD;
};

[domain("tri")]
DomainOut DS(PatchTessTriangle patchTess, float3 uvw : SV_DomainLocation, const OutputPatch<HullOut, 3> triData)
{
	DomainOut dOut;

	dOut.PosW     = triData[0].PosW*uvw.x     + triData[1].PosW*uvw.y     + triData[2].PosW*uvw.z;
	dOut.NormalW  = triData[0].NormalW*uvw.x  + triData[1].NormalW*uvw.y  + triData[2].NormalW*uvw.z;
	dOut.TangentW = triData[0].TangentW*uvw.x + triData[1].TangentW*uvw.y + triData[2].TangentW*uvw.z;
	dOut.Tex      = triData[0].Tex*uvw.x      + triData[1].Tex*uvw.y      + triData[2].Tex*uvw.z;

	dOut.NormalW = normalize(dOut.NormalW);

	const float MipInterval = 20.0f;
	float mipLevel = clamp( (distance(dOut.PosW, gEyePosWorld) - MipInterval) / MipInterval, 0.0f, 6.0f);
	float h = gNormalTexture.SampleLevel(samLinear, dOut.Tex, mipLevel).a;

	dOut.PosW += (gHeightScale*(h-1.0))*dOut.NormalW;

	dOut.PosH = mul(float4(dOut.PosW, 1.0f), gViewProj);

	return dOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////Pixel Shader////////////////////////////////////////////////////

float4 PS(DomainOut pIn, uniform bool gAlphaClip, uniform bool gFogEnable) : SV_Target
{
	float4 texColor = gDiffuseTexture.Sample( samAnisotropic, pIn.Tex );
	if(gAlphaClip) {
		clip(texColor.a - 0.05f);
	}

	pIn.NormalW   = normalize(pIn.NormalW);
	float3 toEyeW = gEyePosWorld - pIn.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

	float3 normalMapSampleT = gNormalTexture.Sample(samLinear, pIn.Tex).rgb;
	float3 bumpedNormalW = processNormalMapToWorld(normalMapSampleT, pIn.NormalW, pIn.TangentW);
	
	float4 ambient, diffuse, spec;
	processLight(gMaterial, pIn.PosW, bumpedNormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = texColor*(ambient + diffuse) + spec;

	if(gFogEnable) {
		float s = saturate((toEyeDistance-gFogStart)/gFogRange);
		litColor = lerp(litColor, gFogColor, s);
	}

	litColor.a = gMaterial.diffuse.a * texColor.a;

	return litColor;
}

float4 PSWave(DomainOut pIn, uniform bool gAlphaClip, uniform bool gFogEnable) : SV_Target
{
	pIn.NormalW   = normalize(pIn.NormalW);
	float3 toEyeW = gEyePosWorld - pIn.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

	float3 normalMapSampleT = gNormalTexture.Sample(samLinear, pIn.Tex).rgb;
	float3 bumpedNormalW = processNormalMapToWorld(normalMapSampleT, pIn.NormalW, pIn.TangentW);
	
	float4 ambient, diffuse, spec;
	processLight(gMaterial, pIn.PosW, bumpedNormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = (ambient + diffuse) + spec;

	if(gFogEnable) {
		float s = saturate((toEyeDistance-gFogStart)/gFogRange);
		litColor = lerp(litColor, gFogColor, s);
	}

	litColor.a = gMaterial.diffuse.a;

	return litColor;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

technique11 TechDisplacement
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

technique11 TechDisplacementWave
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VSWave() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSWave(true, false) ) );
    }
}
