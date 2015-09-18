#include "lightUtil.fx"
#include "util.fx"

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	row_major float4x4 World  : WORLD;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION; //vertex position in homogeneous space
	float3 PosW    : POSITION;    //vertex position in world space
	float3 NormalW : NORMAL;      //vertex normal in world space
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.PosW = mul(float4(vIn.PosL, 1.0f), vIn.World).xyz;
	vOut.NormalW = mul(vIn.NormalL, (float3x3)vIn.World).xyz;

	vOut.PosH = mul(float4(vOut.PosW, 1.0f), gViewProj);

	return vOut;
}

float4 PS(VertexOut pIn) : SV_Target
{
	pIn.NormalW = normalize(pIn.NormalW);
	float3 toEyeW = normalize(gEyePosWorld - pIn.PosW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//light result
	float4 A, D, S;
	int i;

	for(i = 0; i < gDirectionalLightCount; ++i) {
		ProcessDirectionalLight(gMaterial, gDirectionalLight[i], pIn.NormalW, toEyeW, A, D, S);

		ambient += A;
		diffuse += D;
		spec    += S;
	}

	for(i = 0; i < gPointLightCount; ++i) {
		ProcessPointLight(gMaterial, gPointLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);

		ambient += A;
		diffuse += D;
		spec    += S;
	}

	for(i = 0; i < gSpotLightCount; ++i) {
		processSpotLight(gMaterial, gSpotLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);

		ambient += A;
		diffuse += D;
		spec    += S;
	}

	float4 litColor = ambient + diffuse + spec;
	litColor.a = gMaterial.diffuse.a;
	return litColor;
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
