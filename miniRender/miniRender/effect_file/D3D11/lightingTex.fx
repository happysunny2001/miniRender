#include "lightUtil.fx"
#include "util.fx"

struct VertexIn
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex     : TEXCOORD;
};

struct VertexOut
{
    float4 PosH    : SV_POSITION; //vertex position in homogeneous space
    float3 PosW    : POSITION;    //vertex position in world space
    float3 NormalW : NORMAL;      //vertex normal in world space
    float2 Tex     : TEXCOORD;    //pixel texture coord
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.PosW    = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.PosH    = mul(float4(vIn.PosL, 1.0f), gMatWorldViewProj);
	vOut.NormalW = mul(float4(vIn.NormalL, 0.0f), gMatWorldInvTranspose).xyz;
	vOut.Tex     = vIn.Tex;//mul(float4(vIn.Tex, 0.0f, 1.0f), gDiffTexTransform).xy;

	return vOut;
}

float4 PS(VertexOut pIn, uniform bool gAlphaClip, uniform bool gFogEnable) : SV_Target
{
	//float4 texColor = gDiffuseTexture.Sample( samAnisotropic, pIn.Tex );
	float4 texColor = gTexture0.Sample( samAnisotropic, pIn.Tex );
	if(gAlphaClip) {
		clip(texColor.a - 0.05f);
	}

	pIn.NormalW   = normalize(pIn.NormalW);
	float3 toEyeW = gEyePosWorld - pIn.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

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

	float4 litColor = texColor*(ambient + diffuse) + spec;
	
	if(gFogEnable) {
		float s = saturate((toEyeDistance-gFogStart)/gFogRange);
		litColor = lerp(litColor, gFogColor, s);
	}

	litColor.a = gMaterial.diffuse.a * texColor.a;
	return litColor;
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false, false) ) );
    }
}

technique11 LightTechAlphaClip
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false) ) );
    }
}

technique11 LightTechFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false, true) ) );
    }
}

technique11 LightTechAlphaClipFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, true) ) );
    }
}

