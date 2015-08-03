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

Texture2D gTexture0;
Texture2D gTexture1;
Texture2D gTexture2;
Texture2D gTexture3;
Texture2D gTexture4;
Texture2D gTexture5;
Texture2D gTexture6;
Texture2D gTexture7;
