#ifndef __GEOMETRY_TEST_HLSL__
#define __GEOMETRY_TEST_HLSL__

bool AabbAabbCollide(float3 minPoint1, float3 maxPoint1, float3 minPoint2, float3 maxPoint2)
{
	float3 min1 = float3(minPoint1.xyz);
	float3 max1 = maxPoint1.xyz;
	float3 min2 = minPoint2.xyz;
	float3 max2 = float3(maxPoint2.xyz);

	if(any(min1 >= max2)) return false;
	//if(any(maxPoint1 <= minPoint2)) return false;

	//if(minPoint1.x >= maxPoint2.x || minPoint1.y >= maxPoint2.y || minPoint1.z >= maxPoint2.z) return false;
	//if(maxPoint1.x <= minPoint2.x || maxPoint1.y <= minPoint2.y || maxPoint1.z <= minPoint2.z) return false;

	return true;
}

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

#endif
