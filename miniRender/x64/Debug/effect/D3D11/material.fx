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

SamplerState samColor
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState samNormal
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerComparisonState samShadow
{
	Filter   = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComparisonFunc = LESS_EQUAL;
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

float4x4 gDiffTexTransform;

Texture2D gDiffuseTexture;
Texture2D gNormalTexture;
Texture2D gShadowMapTexture;

TextureCube gSkyCubeMap;

TextureCube gReflectCubeMap;
TextureCube gDynamicReflectCubeMap;
cbuffer cbReflectFactor
{
	float fReflectFactor;
};
