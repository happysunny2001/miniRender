#ifndef __GBUFFER_TEXTURE_HLSL__
#define __GBUFFER_TEXTURE_HLSL__

Texture2DMS<float4, MSAA_SAMPLES> gNormalTexture;
Texture2DMS<float4, MSAA_SAMPLES> gDiffuseTexture;
Texture2DMS<float4, MSAA_SAMPLES> gSpecularTexture;
Texture2DMS<float4, MSAA_SAMPLES> gDepthTexture;

#endif
