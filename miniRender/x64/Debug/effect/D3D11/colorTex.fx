#include "material.fx"
#include "util.fx"
#include "VertexLayout.hlsl"

//struct VertexIn
//{
//	float3 PosL  : POSITION;
//	float2 Tex   : TEXCOORD;
//};

///////////////////////////////////////////Vertex///////////////////////////////////////////
struct VertexPosTexOut
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

struct VertexPosTexColorOut
{
    float4 PosH  : SV_POSITION;
    float2 Tex   : TEXCOORD;
    float4 Color : COLOR;
};

struct VertexPosColorOut
{
    float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

//////////////////////////////////////////////VS///////////////////////////////////////////
VertexPosTexOut VS(VertexPosTex vin)
{
	VertexPosTexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gMatWorldViewProj);
	vout.Tex   = vin.Tex;
    
	return vout;
}

VertexPosTexColorOut VSPosTexColor(VertexPosTexColor vin)
{
    VertexPosTexColorOut vout;
    
    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosW, 1.0f), gViewProj);
    vout.Tex   = vin.Tex;
    vout.Color = vin.Color;
    
    return vout;
}

VertexPosColorOut VSPosColor(VertexPosColor vin)
{
    VertexPosColorOut vout;
    
    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gMatWorldViewProj);
    
    // Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

//////////////////////////////////////////////PS///////////////////////////////////////////
float4 PS(VertexPosTexOut pin) : SV_Target
{
	return gDiffuseTexture.Sample( samLinear, pin.Tex );
}

float4 PSGray(VertexPosTexOut pin) : SV_Target
{
	float4 color = gDiffuseTexture.Sample( samLinear, pin.Tex ).r;
	return float4(color.rrr, 1);
}

float4 PSRG(VertexPosTexOut pin) : SV_Target
{
	float4 color = gDiffuseTexture.Sample( samLinear, pin.Tex );
	return float4(color.r, color.g, 0.0, 1.0);
}

float4 PSPosTexColor(VertexPosTexColorOut pin) : SV_Target
{
    float4 texColor = gDiffuseTexture.Sample( samAnisotropic, pin.Tex );
    return texColor*pin.Color;
}

float4 PSPosColor(VertexPosColorOut pin) : SV_Target
{
    return pin.Color;
}

//////////////////////////////////////////////Tech///////////////////////////////////////////
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

technique11 GrayTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSGray() ) );
    }
}

technique11 RGTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSRG() ) );
    }
}

technique11 TechPosTexColor
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VSPosTexColor() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSPosTexColor() ) );
    }
}

technique11 TechPosColor
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VSPosColor() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSPosColor() ) );
    }
}
