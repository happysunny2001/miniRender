#include "util.fx"
#include "lightUtil.fx"

cbuffer cbPerFrame
{
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMaxTessFactor;
	float gMinTessFactor;
};

struct VertexInPNTT
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float2 Tex      : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOutPNTT
{
    float3 PosW      : POSITION;    //vertex position in world space
    float3 NormalW   : NORMAL;      //vertex normal in world space
    float2 Tex       : TEXCOORD;    //pixel texture coord
    float3 TangentW  : TANGENT;     //vertex tangent in world space
    float TessFactor : TESS;        //tessellation factor base on vertex and view position in world space
};

VertexOutPNTT VS(VertexInPNTT vIn)
{
	VertexOutPNTT vOut;

	vOut.PosW     = mul(float4(vIn.PosL, 1.0f), gMatWorld).xyz;
	vOut.NormalW  = mul(float4(vIn.NormalL, 0.0f), gMatWorldInvTranspose).xyz;
	vOut.TangentW = mul(float4(vIn.TangentL, 0.0f), gMatWorld).xyz;
	vOut.Tex      = vIn.Tex;//mul(float4(vIn.Tex, 0.0f, 1.0f), gDiffTexTransform).xy;

	float dist = distance(vOut.PosW, gEyePosWorld);
	float tess = saturate(gMinTessDistance-dist/(gMinTessDistance-gMaxTessDistance));
	vOut.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);

	return vOut;
}
