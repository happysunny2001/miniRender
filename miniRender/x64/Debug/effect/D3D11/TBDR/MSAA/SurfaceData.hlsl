#ifndef __SURFACE_DATA_HLSL__
#define __SURFACE_DATA_HLSL__

struct SurfaceData
{
	float4 positionView;
	float4 positionWorld;
	float3 normalView;
	float4 diffuse;
	float4 specular;
};

#endif
