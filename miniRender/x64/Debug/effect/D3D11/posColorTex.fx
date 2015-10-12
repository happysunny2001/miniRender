#include "material.fx"
#include "util.fx"

struct VertexIn
{
	float3 PosL  : POSITION;
	float2 Tex   : TEXCOORD;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gMatWorldViewProj);
	vout.Tex   = vin.Tex;
	vout.Color = vin.Color;
    
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 texColor = gTexture0.Sample( samAnisotropic, pin.Tex );
	return texColor*pin.Color;
}

technique11 
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}