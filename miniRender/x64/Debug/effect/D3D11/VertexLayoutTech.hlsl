#ifndef __VERTEX_LAYOUTTECH_HLSL__
#define __VERTEX_LAYOUTTECH_HLSL__

#include "VertexLayout.hlsl"

/////////////////////////////////////////VS//////////////////////////////////////////////
VertexOut VSPos(VertexPos vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

VertexOut VSPosColor(VertexPosColor vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

VertexOut VSPosNormal(VertexPosNormal vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

VertexOut VSPosNormalTex(VertexPosNormalTex vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

VertexOut VSPosNormalTexTan(VertexPosNormalTexTan vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

VertexOut VSPosTex(VertexPosTex vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

VertexOut VSPosTexColor(VertexPosTexColor vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosW, 1.0f);
	return vout;
}

VertexOut VSBatchPosNormal(VertexBatchPosNormal vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

///////////////////////////////////Tech////////////////////////////////////////////////

technique11 techPos
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPos() ) );
		SetPixelShader( NULL );
	}
}

technique11 techPosColor
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPosColor() ) );
		SetPixelShader( NULL );
	}
}

technique11 techPosNormal
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPosNormal() ) );
		SetPixelShader( NULL );
	}
}

technique11 techPosNormalTex
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPosNormalTex() ) );
		SetPixelShader( NULL );
	}
}

technique11 techPosNormalTexTan
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPosNormalTexTan() ) );
		SetPixelShader( NULL );
	}
}

technique11 techPosTex
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPosTex() ) );
		SetPixelShader( NULL );
	}
}

technique11 techPosTexColor
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSPosTexColor() ) );
		SetPixelShader( NULL );
	}
}

technique11 techBatchPosNormal
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VSBatchPosNormal() ) );
		SetPixelShader( NULL );
	}
}

#endif