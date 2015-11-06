#include "util.fx"
#include "lightUtil.fx"

struct VertexInPNTT
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float2 Tex      : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOutPNTT
{
    float4 PosH     : SV_POSITION; //vertex position in homogeneous space
    float3 PosW     : POSITION;    //vertex position in world space
    float3 NormalW  : NORMAL;      //vertex normal in world space
    float2 Tex      : TEXCOORD;    //pixel texture coord
    float3 TangentW : TANGENT;     //vertex tangent in world space
};

VertexOutPNTT VS(VertexInPNTT vIn)
{
	VertexOutPNTT vOut;

	vOut.PosW     = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.PosH     = mul(float4(vIn.PosL, 1.0f), gMatWorldViewProj);
	vOut.NormalW  = mul(float4(vIn.NormalL, 0.0f), gMatWorldInvTranspose).xyz;
	vOut.TangentW = mul(float4(vIn.TangentL, 0.0f), gMatWorld).xyz;
	vOut.Tex      = vIn.Tex;//mul(float4(vIn.Tex, 0.0f, 1.0f), gDiffTexTransform).xy;

	return vOut;
}

float4 PS(VertexOutPNTT pIn, uniform bool gAlphaClip, uniform bool gFogEnable) : SV_Target
{
	float4 texColor = gDiffuseTexture.Sample( samAnisotropic, pIn.Tex );
	if(gAlphaClip) {
		clip(texColor.a - 0.05f);
	}

	pIn.NormalW   = normalize(pIn.NormalW);
	float3 toEyeW = gEyePosWorld - pIn.PosW;
	float toEyeDistance = length(toEyeW);
	toEyeW /= toEyeDistance;

	float3 normalMapSampleT = gNormalTexture.Sample(samLinear, pIn.Tex).rgb;
	float3 bumpedNormalW = processNormalMapToWorld(normalMapSampleT, pIn.NormalW, pIn.TangentW);
	
	float4 ambient, diffuse, spec;
	processLight(gMaterial, pIn.PosW, bumpedNormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = texColor*(ambient + diffuse) + spec;

	if(gFogEnable) {
		float s = saturate((toEyeDistance-gFogStart)/gFogRange);
		litColor = lerp(litColor, gFogColor, s);
	}

	litColor.a = gMaterial.diffuse.a * texColor.a;

	return litColor;
}

technique11 NormalTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false) ) );
    }
}
