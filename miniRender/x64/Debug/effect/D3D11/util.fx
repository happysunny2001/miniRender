cbuffer cbTransform
{
	float4x4 gMatWorld;
	float4x4 gMatWorldInvTranspose; //transform normal
	float4x4 gMatWorldViewProj;
	float4x4 gViewProj;
	float3 gEyePosWorld;	      //camera position in world space
};

bool AabbBehindPlane(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);

	float r = dot(extents, n);
	float s = dot(float4(center, -1.0f), plane);

	return (s+r) < 0.0f;
}

bool AabbOutsideFrustum(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for(int i = 0; i < 6; ++i) {
		if(AabbBehindPlane(center, extents, frustumPlanes[i])) return true;
	}

	return false;
}
