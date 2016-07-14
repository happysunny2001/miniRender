#include "lightUtil.fx"
#include "util.fx"

//SamplerState SampleTypePoint : register(s0);

cbuffer cbDeferredTransform
{
	float4x4 gMatInvertViewProj;
	float3 gDefaultCameraPosWorld;
};

SamplerState samDepth
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

Texture2D gSpecularTexture;
Texture2D gDepthTexture;

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

half3 decode (half4 enc)
{
    half4 nn = enc*half4(2,2,0,0) + half4(-1,-1,1,-1);
    half l = dot(nn.xyz,-nn.xyw);
    nn.z = l;
    nn.xy *= half(sqrt(l));
    return nn.xyz * 2 + half3(0,0,-1);
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 dcolor = gDiffuseTexture.Sample( samColor, pin.Tex );
	float4 normal = gNormalTexture.Sample( samNormal, pin.Tex );
	float4 dspec = gSpecularTexture.Sample( samColor, pin.Tex );
	float depth = gDepthTexture.Sample( samDepth, pin.Tex ).r;
	float4 position;

	position = float4(pin.Tex.x * 2.0f - 1.0f, -(pin.Tex.y * 2.0f - 1.0f), depth, 1.0f);
	position = mul(position, gMatInvertViewProj);
	position /= position.w;

	float3 toEyeW = normalize(gDefaultCameraPosWorld - position.xyz);

	Material mat;
	mat.ambient = float4(0, 0, 0, 0);
	mat.diffuse = dcolor;
	mat.specular = dspec;
	mat.reflect = float4(1.0, 1.0, 1.0, 1.0);

	float4 ambient, diffuse, spec;
	
	float3 n = float3(decode(half4(normal.xyzw)).xyz);
	processLight(mat, position.xyz, n.xyz, toEyeW, ambient, diffuse, spec);
	float4 litColor = ambient + diffuse + spec;
	litColor.a = 1.0;

	return litColor;
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
