//material structure
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; //w:Spec Power
	float4 reflect;
};

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

cbuffer cbFog
{
	float  gFogStart;             //start fog distance
	float  gFogRange;             //fog range
	float4 gFogColor;             //fog color
};

cbuffer cbMaterial
{
	Material gMaterial;             //material of object
};

Texture2D gDiffuseTexture;
Texture2D gNormalTexture;

TextureCube gSkyCubeMap;

TextureCube gReflectCubeMap;
TextureCube gDynamicReflectCubeMap;
cbuffer cbReflectFactor
{
	float fReflectFactor;
};
