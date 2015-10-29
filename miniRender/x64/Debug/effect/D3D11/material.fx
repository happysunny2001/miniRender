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

Texture2D gTexture0;
Texture2D gTexture1;
Texture2D gTexture2;
Texture2D gTexture3;
Texture2D gTexture4;
Texture2D gTexture5;
Texture2D gTexture6;
Texture2D gTexture7;

TextureCube gSkyCubeMap;

TextureCube gReflectCubeMap;
cbuffer cbReflectFactor
{
	float fReflectFactor;
};
