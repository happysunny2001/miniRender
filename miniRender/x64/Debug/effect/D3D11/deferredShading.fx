#include "material.fx"
#include "util.fx"

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION; //vertex position in homogeneous space
	float3 NormalW : NORMAL;      //vertex normal in world space
	float2 Depth   : TEXCOORD0;
};

struct VertexOutClear
{
	float4 PosH    : SV_POSITION; //vertex position in homogeneous space
};

struct PixelOut
{
	float2 Normal : SV_Target0;
	float4 Diffuse : SV_Target1;
	float4 Specular : SV_Target2;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), gMatWorldViewProj);
	vOut.NormalW = mul(float4(vIn.NormalL, 0.0f), gMatWorldInvTranspose).xyz;
	vOut.Depth = float2(vOut.PosH.z, vOut.PosH.w);

	return vOut;
}

VertexOutClear VSClear(VertexIn vIn)
{
	VertexOutClear vOut;
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), gMatWorldViewProj);
	return vOut;
}

half2 encode (half3 n)
{	
	float2 ff = normalize(n.xy) * (sqrt(-n.z*0.5+0.5));
	half2 enc = half2(ff.xy);
	enc = enc*0.5+0.5;
	return enc;
}

PixelOut PS(VertexOut pIn)
{
	PixelOut pOut;
	
	half2 pn = encode(half3(normalize(pIn.NormalW.xyz)));
	pOut.Normal.rg = pn.xy;

	pOut.Diffuse = gMaterial.diffuse;
	pOut.Specular = gMaterial.specular;

	return pOut;
}

PixelOut PSClear(VertexOutClear pIn)
{
	PixelOut pOut;
	pOut.Normal.rg = float2(0.5, 0.5);
	pOut.Diffuse = float4(0.0, 0.0, 0.0, 0.0);
	pOut.Specular = float4(0.0, 0.0, 0.0, 0.0);
	return pOut;
}

technique11 DeferredTech
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

technique11 DeferredClearTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSClear() ) );
		SetHullShader( NULL );
		SetDomainShader( NULL );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PSClear() ) );
	}
}
