#ifndef __SSAO_TECHNIQUE_H__
#define __SSAO_TECHNIQUE_H__

#include "SSAO.hlsl"
#include "SSAOBlur.hlsl"

technique11 TechSSAO
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SSAO_VS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SSAO_PS() ) );
	}
}

technique11 TechSSAOHorizBlur
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_5_0, SSAO_Blur_VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SSAO_Blur_PS(true) ) );
	}
}

technique11 TechSSAOVertBlur
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_5_0, SSAO_Blur_VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SSAO_Blur_PS(false) ) );
	}
}

#endif