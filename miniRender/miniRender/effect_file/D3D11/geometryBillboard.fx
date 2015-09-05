#include "lightUtil.fx"
#include "util.fx"

cbuffer cbFixed
{
	float2 gTexC[4] = {
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};

Texture2DArray gTreeMapArray;

struct VertexIn
{
	float3 PosW  : POSITION;
	float2 SizeW : TEXCOORD;
};

struct VertexOut
{
	float3 CenterW : POSITION;
	float2 SizeW : TEXCOORD;
};

struct GeoOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
	uint PrimID : SV_PrimitiveID;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.CenterW = vin.PosW;
	vout.SizeW = vin.SizeW;
	return vout;
}

[maxvertexcount(4)]
void GS(point VertexOut gin[1], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = gEyePosWorld - gin[0].CenterW;
	look.y = 0;
	look = normalize(look);
	float3 right = cross(up, look);

	float fHalfWidth  = gin[0].SizeW.x*0.5f;
	float fHalfHeight = gin[0].SizeW.y*0.5f;
	float4 v[4];
	v[0] = float4(gin[0].CenterW + fHalfWidth*right - fHalfHeight*up, 1.0f);
	v[1] = float4(gin[0].CenterW + fHalfWidth*right + fHalfHeight*up, 1.0f);
	v[2] = float4(gin[0].CenterW - fHalfWidth*right - fHalfHeight*up, 1.0f);
	v[3] = float4(gin[0].CenterW - fHalfWidth*right + fHalfHeight*up, 1.0f);

	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gViewProj);
		gout.PosW = v[i].xyz;
		gout.NormalW = look;
		gout.Tex = gTexC[i];
		gout.PrimID = primID;

		triStream.Append(gout);
	}
}

float4 PS(GeoOut pin) : SV_Target
{
	float3 uvw = float3(pin.Tex, pin.PrimID%4);
	float4 texColor = gTreeMapArray.Sample( samLinear, uvw );
	clip(texColor.a - 0.05f);

	pin.NormalW = normalize(pin.NormalW);
	float3 toEyeW = gEyePosWorld - pin.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//light result
	float4 A, D, S;
	int i;
	
	if(gDirectionalLightCount > 0)
	{
		[unroll]
		for(i = 0; i < gDirectionalLightCount; ++i) {
			ProcessDirectionalLight(gMaterial, gDirectionalLight[i], pin.NormalW, toEyeW, A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}
	}
	
	if(gPointLightCount > 0)
	{
		[unroll]
		for(i = 0; i < gPointLightCount; ++i) {
			ProcessPointLight(gMaterial, gPointLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}
	}
	
	if(gSpotLightCount > 0)
	{
		[unroll]
		for(i = 0; i < gSpotLightCount; ++i) {
			processSpotLight(gMaterial, gSpotLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}
	}

	float4 litColor = texColor*(ambient + diffuse) + spec;

	litColor.a = gMaterial.diffuse.a * texColor.a;
	return litColor;
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}


