#ifndef __MATERIAL_HLSL__
#define __MATERIAL_HLSL__

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; //w:Spec Power
	float4 reflect;
};

cbuffer cbMaterial
{
	Material gMaterial;             //material of object
};

#endif
