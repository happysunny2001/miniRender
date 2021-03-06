#include "material.fx"
#include "util.fx"
#include "VertexLayout.hlsl"

//struct VertexIn
//{
//	float3 PosW  : POSITION;
//	float2 Tex   : TEXCOORD;
//	float4 Color : COLOR;
//};

struct VertexPosTexColorOut
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
	float4 Color : COLOR;
};

VertexPosTexColorOut VS(VertexPosTexColor vin)
{
	VertexPosTexColorOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosW, 1.0f), gViewProj);
	vout.Tex   = vin.Tex;
	vout.Color = vin.Color;
    
	return vout;
}

float4 PS(VertexPosTexColorOut pin) : SV_Target
{
	float4 texColor = gDiffuseTexture.Sample( samAnisotropic, pin.Tex );
	return texColor*pin.Color;
}

technique11 Tech01
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
