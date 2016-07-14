#ifndef __PER_FRAME_CONSTANT_HLSL__
#define __PER_FRAME_CONSTANT_HLSL__

cbuffer PerFrameConstants : register(b0)
{
	float4x4 gMatWorld;
	float4x4 gMatWorldInvTranspose; //transform normal
	float4x4 gMatViewProj;
	float4x4 gMatView;
	float4x4 gMatProj;
	float3   gEyePosWorld;	       //camera position in world space
	float4   gCameraNearFarZ;      //x:Far Plane Z value, y:Near Plane Z value
	uint4   gFrameBufferDimensions;
};

#endif
