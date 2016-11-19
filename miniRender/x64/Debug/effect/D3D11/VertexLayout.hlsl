#ifndef __VERTEX_LAYOUT_HLSL__
#define __VERTEX_LAYOUT_HLSL__

struct VertexPos
{
	float3 PosL  : POSITION;
};

struct VertexPosColor
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexPosNormal
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexPosNormalTex
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex     : TEXCOORD;
};

struct VertexPosNormalTexTan
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float2 Tex      : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexPosTex
{
	float3 PosL  : POSITION;
	float2 Tex   : TEXCOORD;
};

struct VertexPosTexColor
{
	float3 PosW  : POSITION;
	float2 Tex   : TEXCOORD;
	float4 Color : COLOR;
};

struct VertexBatchPosNormal
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	row_major float4x4 World  : WORLD;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
};

#endif
