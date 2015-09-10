#include "util.fx"

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosL  : POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.PosL = vin.PosL;
	vout.Color = vin.Color;

	return vout;
}

struct PatchTess
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess    : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	float3 centerL = 0.25f*(patch[0].PosL + patch[1].PosL + patch[2].PosL + patch[3].PosL);
	float3 centerW = mul(float4(centerL, 1.0f), gMatWorld).xyz;
	
	float d = distance(centerW, gEyePosWorld);

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	
	const float d0 = 20.0f;
	const float d1 = 100.0f;
	float tess = 64.0f*saturate( (d1-d)/(d1-d0) );

	// Uniformly tessellate the patch.

	pt.EdgeTess[0] = tess;
	pt.EdgeTess[1] = tess;
	pt.EdgeTess[2] = tess;
	
	pt.InsideTess = tess;
	
	return pt;
}

struct HullOut
{
	float4 PosL  : POSITION;
	float4 Color : COLOR;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p, 
		   uint i : SV_OutputControlPointID,
		   uint patchId : SV_PrimitiveID)
{
	HullOut hOut;
	hOut.PosL = p[i].PosL;
	hOut.Color = p[i].Color;
	return hOut;
}

struct DomainOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

[domain("tri")]
DomainOut DS(PatchTess patchTess, float3 uvw : SV_DomainLocation, const OutputPatch<HullOut, 3> triangle)
{
	DomainOut dOut;

	float3 p = triangle[0].PosL*uvw.u + triangle[1].PosL*uvw.v + triangle[2].PosL*uvw.w;
	dOut.PosH = mul(float4(p, 1.0f), gMatWorldViewProj);
	dOut.Color = triangle[0].Color*uvw.u + triangle[1].Color*uvw.v + triangle[2].Color*uvw.w;
	return dOut;
}

float4 PS(DomainOut dOut) : SV_Target
{
	return dOut.Color;
}

technique11 Tess
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

