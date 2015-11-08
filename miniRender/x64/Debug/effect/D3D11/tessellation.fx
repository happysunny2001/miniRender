#include "util.fx"

///////////////////////////////////////Vertex Shader/////////////////////////////////////////////////////
struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.PosL = vin.PosL;
	vout.Color = vin.Color;

	return vout;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////Constant Hull Shader//////////////////////////////////////////////////////////////////
struct PatchTessTriangle
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess    : SV_InsideTessFactor;
};

struct PatchTessQuad
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2]    : SV_InsideTessFactor;
};

PatchTessTriangle ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTessTriangle pt;
	
	float3 centerL = patch[0].PosL;
	float3 centerW = mul(float4(centerL, 1.0f), gMatWorld).xyz;
	
	float d = distance(centerW, gEyePosWorld);

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	
	const float d0 = 20.0f;
	const float d1 = 500.0f;
	float tess = 64.0f*saturate( (d1-d)/(d1-d0) );

	// Uniformly tessellate the patch.

	pt.EdgeTess[0] = tess;
	pt.EdgeTess[1] = tess;
	pt.EdgeTess[2] = tess;
	
	pt.InsideTess = tess;
	
	return pt;
}

PatchTessQuad ConstantHSQuad(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTessQuad pt;
	
	float3 centerL = 0.25f*(patch[0].PosL + patch[1].PosL + patch[2].PosL + patch[3].PosL);
	float3 centerW = mul(float4(centerL, 1.0f), gMatWorld).xyz;
	
	float d = distance(centerW, gEyePosWorld);

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	
	const float d0 = 20.0f;
	const float d1 = 200.0f;
	float tess = 64.0f*saturate( (d1-d)/(d1-d0) );

	// Uniformly tessellate the patch.

	pt.EdgeTess[0] = tess;
	pt.EdgeTess[1] = tess;
	pt.EdgeTess[2] = tess;
	pt.EdgeTess[3] = tess;
	
	pt.InsideTess[0] = tess;
	pt.InsideTess[1] = tess;
	
	return pt;
}

PatchTessQuad ConstantHSBezier(InputPatch<VertexOut, 16> patch, uint patchID : SV_PrimitiveID)
{
	PatchTessQuad pt;
	
	pt.EdgeTess[0] = 25.0f;
	pt.EdgeTess[1] = 25.0f;
	pt.EdgeTess[2] = 25.0f;
	pt.EdgeTess[3] = 25.0f;
	
	pt.InsideTess[0] = 25.0f;
	pt.InsideTess[1] = 25.0f;

	return pt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////Hull Shader////////////////////////////////////////////////
struct HullOut
{
	float3 PosL  : POSITION;
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

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHSQuad")]
[maxtessfactor(64.0f)]
HullOut HSQuad(InputPatch<VertexOut, 4> p, 
		   uint i : SV_OutputControlPointID,
		   uint patchId : SV_PrimitiveID)
{
	HullOut hOut;
	hOut.PosL = p[i].PosL;
	hOut.Color = p[i].Color;
	return hOut;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("ConstantHSBezier")]
[maxtessfactor(64.0f)]
HullOut HSBezier(InputPatch<VertexOut, 16> p, 
		   uint i : SV_OutputControlPointID,
		   uint patchId : SV_PrimitiveID)
{
	HullOut hOut;
	hOut.PosL = p[i].PosL;
	hOut.Color = p[i].Color;
	return hOut;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////Domain Shader////////////////////////////////////////////////////
struct DomainOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

[domain("tri")]
DomainOut DS(PatchTessTriangle patchTess, float3 uvw : SV_DomainLocation, const OutputPatch<HullOut, 3> triData)
{
	DomainOut dOut;

	float3 p = triData[0].PosL*uvw.x + triData[1].PosL*uvw.y + triData[2].PosL*uvw.z;
	dOut.PosH = mul(float4(p, 1.0f), gMatWorldViewProj);
	dOut.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return dOut;
}

[domain("quad")]
DomainOut DSQuad(PatchTessQuad patchTess, float2 uv : SV_DomainLocation, const OutputPatch<HullOut, 4> quadData)
{
	DomainOut dOut;

	float3 v1 = lerp(quadData[0].PosL, quadData[1].PosL, uv.x); 
	float3 v2 = lerp(quadData[2].PosL, quadData[3].PosL, uv.x); 
	float3 p  = lerp(v1, v2, uv.y); 

	p.y = 0.3f*( p.z*sin(p.x) + p.x*cos(p.z) );

	dOut.PosH = mul(float4(p, 1.0f), gMatWorldViewProj);
	dOut.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return dOut;
}

/////////////////////////////////////////////Bezier ///////////////////////////////////////////////////////
float4 BernsteinBasis(float t)
{
    float invT = 1.0f - t;

    return float4( invT * invT * invT,
                   3.0f * t * invT * invT,
                   3.0f * t * t * invT,
                   t * t * t );
}

float3 CubicBezierSum(const OutputPatch<HullOut, 16> bezpatch, float4 basisU, float4 basisV)
{
    float3 sum = float3(0.0f, 0.0f, 0.0f);
    sum  = basisV.x * (basisU.x*bezpatch[0].PosL  + basisU.y*bezpatch[1].PosL  + basisU.z*bezpatch[2].PosL  + basisU.w*bezpatch[3].PosL );
    sum += basisV.y * (basisU.x*bezpatch[4].PosL  + basisU.y*bezpatch[5].PosL  + basisU.z*bezpatch[6].PosL  + basisU.w*bezpatch[7].PosL );
    sum += basisV.z * (basisU.x*bezpatch[8].PosL  + basisU.y*bezpatch[9].PosL  + basisU.z*bezpatch[10].PosL + basisU.w*bezpatch[11].PosL);
    sum += basisV.w * (basisU.x*bezpatch[12].PosL + basisU.y*bezpatch[13].PosL + basisU.z*bezpatch[14].PosL + basisU.w*bezpatch[15].PosL);

    return sum;
}

[domain("quad")]
DomainOut DSBezier(PatchTessQuad patchTess, float2 uv : SV_DomainLocation, const OutputPatch<HullOut, 16> bezPatch)
{
	DomainOut dOut;

	float4 basisU = BernsteinBasis(uv.x);
	float4 basisV = BernsteinBasis(uv.y);

	float3 p  = CubicBezierSum(bezPatch, basisU, basisV);
	
	dOut.PosH = mul(float4(p, 1.0f), gMatWorldViewProj);
	dOut.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return dOut;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////Pixel Shader////////////////////////////////////////////////////
float4 PS(DomainOut dOut) : SV_Target
{
	return dOut.Color;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

technique11 TessTriangle
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

technique11 TessQuad
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HSQuad() ) );
        SetDomainShader( CompileShader( ds_5_0, DSQuad() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

technique11 TessBezier
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HSBezier() ) );
        SetDomainShader( CompileShader( ds_5_0, DSBezier() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

