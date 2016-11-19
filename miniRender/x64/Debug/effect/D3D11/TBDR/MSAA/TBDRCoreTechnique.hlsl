#ifndef __TBDR_CORE_TECHNIQUE_HLSL__
#define __TBDR_CORE_TECHNIQUE_HLSL__

#include "ComputeTiles.hlsl"

technique11 TBDRCore
{
	pass P0
	{
		SetVertexShader( NULL );
        SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, TBDRCS() ) );
	}
}

#endif