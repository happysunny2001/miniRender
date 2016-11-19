#ifndef __SPRITE_RENDER_TECHNIQUE_H__
#define __SPRITE_RENDER_TECHNIQUE_H__

#include "SpriteRender.hlsl"

technique11 SpriteRender
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SpriteRenderVS() ) );
		SetHullShader( NULL );
		SetDomainShader( NULL );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, SpriteRenderPS() ) );
	}
}

technique11 SpriteGrayTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, SpriteRenderVS() ) );
        SetHullShader( NULL );
        SetDomainShader( NULL );
	SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, SpriteRenderGrayPS() ) );
    }
}

#endif
