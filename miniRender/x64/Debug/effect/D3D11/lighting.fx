#include "lightUtil.fx"
#include "util.fx"

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
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
	vOut.PosW = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), gMatWorldViewProj);
	vOut.NormalW = mul(float4(vIn.NormalL, 0.0f), gMatWorldInvTranspose).xyz;

	return vOut;
}

float4 PS(VertexOut pIn) : SV_Target
{
	pIn.NormalW = normalize(pIn.NormalW);
	float3 toEyeW = normalize(gEyePosWorld - pIn.PosW);
	
	float4 ambient, diffuse, spec;

	processLight(gMaterial, pIn.PosW, pIn.NormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = ambient + diffuse + spec;
	litColor.a = gMaterial.diffuse.a;

	return litColor;
}

float4 PSReflect(VertexOut pIn) : SV_Target
{
	pIn.NormalW = normalize(pIn.NormalW);
	float3 toEyeW = normalize(gEyePosWorld - pIn.PosW);

	float4 ambient, diffuse, spec;

	processLight(gMaterial, pIn.PosW, pIn.NormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = ambient + diffuse + spec;
	litColor.a = gMaterial.diffuse.a;

	float4 reflectionColor = processReflection(gMaterial, gReflectCubeMap, pIn.NormalW, toEyeW);

	float factor = saturate(fReflectFactor);
	float4 finalColor = reflectionColor*factor + litColor*(1.0f-factor);
	finalColor.a = gMaterial.diffuse.a;

	return finalColor;
}

float4 PSDynamicReflect(VertexOut pIn) : SV_Target
{
	pIn.NormalW = normalize(pIn.NormalW);
	float3 toEyeW = normalize(gEyePosWorld - pIn.PosW);
	
	float4 ambient, diffuse, spec;

	processLight(gMaterial, pIn.PosW, pIn.NormalW, toEyeW, ambient, diffuse, spec);
	float4 litColor = ambient + diffuse + spec;
	litColor.a = gMaterial.diffuse.a;

	float4 dynamicReflectionColor = processReflection(gMaterial, gDynamicReflectCubeMap, pIn.NormalW, toEyeW);

	float factor = saturate(fReflectFactor);
	float4 finalColor = dynamicReflectionColor*factor + litColor*(1.0f-factor);
	finalColor.a = gMaterial.diffuse.a;

	return finalColor;
}

technique11 LightTech
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

technique11 LightTechReflect
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSReflect() ) );
    }
}

technique11 LightTechDynamicReflect
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSDynamicReflect() ) );
    }
}

