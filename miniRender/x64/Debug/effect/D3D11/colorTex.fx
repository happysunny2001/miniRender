#include "material.fx"
#include "util.fx"

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

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gMatWorldViewProj);
	vout.Tex   = vin.Tex;
    
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return gDiffuseTexture.Sample( samAnisotropic, pin.Tex );
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
