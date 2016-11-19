#ifndef __TBDR_TECHNIQUE_HLSL__
#define __TBDR_TECHNIQUE_HLSL__

#include "GBuffer.hlsl"
//#include "ComputeTiles.hlsl"
#include "ScreenQuad.hlsl"

technique11 GBufferClear
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, ClearGBufferVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, ClearGBufferPS() ) );
	}
}

technique11 GBufferBuild
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, BuildGBufferVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, BuildGBufferPS() ) );
	}
}

technique11 ScreenQuad
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, ScreenQuadVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, ScreenQuadPS() ) );
	}
}

technique11 ScreenQuadRG
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, ScreenQuadVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, ScreenQuadRGPS() ) );
	}
}

technique11 ScreenQuadGray
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, ScreenQuadVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, ScreenQuadGrayPS() ) );
	}
}

technique11 TechTest
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, TestVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, TestPS() ) );
	}
}

#endif