cbuffer cbTransform
{
	float4x4 gMatWorld;
	float4x4 gMatWorldInvTranspose; //transform normal
	float4x4 gMatWorldViewProj;
	float4x4 gViewProj;
	float3 gEyePosWorld;	      //camera position in world space
};