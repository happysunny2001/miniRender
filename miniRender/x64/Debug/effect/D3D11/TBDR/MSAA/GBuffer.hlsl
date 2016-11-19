#ifndef __GBUFFER_HLSL__
#define __GBUFFER_HLSL__

#include "PerFrameConstant.hlsl"
#include "Material.hlsl"
#include "SurfaceData.hlsl"
#include "TBDRDefines.hlsl"

struct GBuffer
{
	float2 NormalXY       : SV_Target0; //normal in view space
	float4 DiffuseRGB     : SV_Target1; 
	float4 Specular_Power : SV_Target2;
};

Texture2DMS<float4, MSAA_SAMPLES> gNormalTexture;
Texture2DMS<float4, MSAA_SAMPLES> gDiffuseTexture;
Texture2DMS<float4, MSAA_SAMPLES> gSpecularTexture;
Texture2DMS<float4, MSAA_SAMPLES> gDepthTexture;

struct VertexVSInput
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexVSOutput
{
	float4 PosH    : SV_POSITION; //vertex position in homogeneous space
	//float4 PositionView : VIEW_POSITION; 
	float3 NormalW : NORMAL;      //vertex normal in world space
};

struct VertexClearVSInput
{
	float3 PosL  : POSITION;
	float2 Tex   : TEXCOORD;
};

struct VertexClearVSOutput
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

float2 encodeNormal (float3 n)
{	
	float2 ff = normalize(n.xy) * (sqrt(-n.z*0.5+0.5));
	float2 enc = ff.xy;
	enc = enc*0.5+0.5;
	return enc;
}

float3 decodeNormal (float4 enc)
{
    float4 nn = enc*float4(2,2,0,0) + float4(-1,-1,1,-1);
    float l = dot(nn.xyz,-nn.xyw);
    nn.z = l;
    nn.xy *= sqrt(l);
    return nn.xyz * 2 + float3(0,0,-1);
}

float2 EncodeSphereMap(float3 n)
{
    float oneMinusZ = 1.0f - n.z;
    float p = sqrt(n.x * n.x + n.y * n.y + oneMinusZ * oneMinusZ);
    return n.xy / p * 0.5f + 0.5f;
}

float3 DecodeSphereMap(float2 e)
{
    float2 tmp = e - e * e;
    float f = tmp.x + tmp.y;
    float m = sqrt(4.0f * f - 1.0f);
    
    float3 n;
    n.xy = m * (e * 4.0f - 2.0f);
    n.z  = 3.0f - 8.0f * f;
    return n;
}

float3 ComputePositionViewFromZ(float2 positionScreen, float viewSpaceZ)
{
	float2 screenSpaceRay = float2(positionScreen.x / gMatProj._11,
                                   positionScreen.y / gMatProj._22);
    
    float3 positionView;
    positionView.z = viewSpaceZ;
    // Solve the two projection equations
    positionView.xy = screenSpaceRay.xy * positionView.z;
    
    return positionView;
}

SurfaceData ComputeSurfaceFromGBuffer(uint2 globalCoords, uint sampleIndex)
{
	SurfaceData sData;

	sData.diffuse  = gDiffuseTexture.Load(int2(globalCoords.xy), sampleIndex);
	sData.specular = gSpecularTexture.Load(int2(globalCoords.xy), sampleIndex);
	float depth = gDepthTexture.Load(int2(globalCoords.xy), sampleIndex).x;
	float4 normal = gNormalTexture.Load(int2(globalCoords.xy), sampleIndex);

	sData.normalView = normalize(DecodeSphereMap(normal.xy));
	//sData.normalView = decodeNormal(normal);
	
	float2 dim;
	uint temp;
	gDepthTexture.GetDimensions(dim.x, dim.y, temp);

	float2 screenPixelOffset = float2(2.0f, -2.0f) / dim;
    float2 positionScreen = (float2(globalCoords.xy) + 0.5f) * screenPixelOffset.xy + float2(-1.0f, 1.0f);
    float2 positionScreenX = positionScreen + float2(screenPixelOffset.x, 0.0f);
    float2 positionScreenY = positionScreen + float2(0.0f, screenPixelOffset.y);

    float viewSpaceZ = gMatProj._43 / (depth - gMatProj._33);
    sData.positionView = float4(ComputePositionViewFromZ(positionScreen, viewSpaceZ).xyz, 1.0f);
    sData.positionWorld = mul(sData.positionView, gMatViewInv);

	return sData;
}

void ComputeSurfaceFromGBufferAllSamples(uint2 globalCoords, out SurfaceData surface[MSAA_SAMPLES])
{
	[unroll]
	for(uint i = 0; i < MSAA_SAMPLES; ++i) {
		surface[i] = ComputeSurfaceFromGBuffer(globalCoords, i);
	}
}

float3 ComputeFaceNormal(float3 position)
{
    return cross(ddx_coarse(position), ddy_coarse(position));
}

VertexVSOutput BuildGBufferVS(VertexVSInput input)
{
	VertexVSOutput output;

	float4x4 matWorldViewProj = mul(gMatWorld, gMatViewProj);
	output.PosH = mul(float4(input.PosL, 1.0f), matWorldViewProj);

	//float4x4 matWorldView = mul(gMatWorld, gMatView);
	output.NormalW = mul(float4(input.NormalL, 0.0f), gMatWorldInvTranspose).xyz;
	//output.PositionView = mul(float4(input.PosL, 1.0f), matWorldView);

	return output;
}

GBuffer BuildGBufferPS(VertexVSOutput vOutput)
{
	GBuffer gBuffer;

	//float3 faceNormal = ComputeFaceNormal(vOutput.PositionView.xyz);
	float3 normalV = mul(float4(vOutput.NormalW, 0.0), gMatView).xyz;

	gBuffer.NormalXY = EncodeSphereMap(normalize(normalV));
	//gBuffer.NormalXY = encodeNormal(normalize(vOutput.NormalW));
	gBuffer.DiffuseRGB = gMaterial.diffuse;
	gBuffer.Specular_Power = gMaterial.specular;

	return gBuffer;
}

VertexClearVSOutput ClearGBufferVS(VertexClearVSInput input)
{
	VertexClearVSOutput output;

	float4x4 matWorldViewProj = mul(gMatWorld, gMatViewProj);
	output.PosH = mul(float4(input.PosL, 1.0f), matWorldViewProj);
	output.Tex  = input.Tex;

	return output;
}

GBuffer ClearGBufferPS(VertexClearVSOutput vOutput)
{
	GBuffer gBuffer;

	gBuffer.NormalXY.xy = float2(0.5f, 0.5f);
	gBuffer.DiffuseRGB  = float4(0.0f, 0.0f, 0.0f, 1.0f);
	gBuffer.Specular_Power = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return gBuffer;
}

#endif
