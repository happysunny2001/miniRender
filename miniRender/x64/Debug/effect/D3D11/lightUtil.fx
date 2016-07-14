#include "material.fx"

//Light Util
//support directional light, point light, spot light

//directional light structure
struct DirectionalLight
{
	float4 direction;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 lightAttr; //x:shadow flag,0:not cast shadow,1:cast shadow
};

//point light structure
struct PointLight
{
	float4 position;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 att; //x,y,z:attenuation A0, A1, A2; w:range
};

//spot light structure
struct SpotLight
{
	float4 position;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 att; //x,y,z:attenuation A0, A1, A2; w:range
	float4 spotDirection; //x, y, z:spot light position; w:spot
};

cbuffer cbLighting
{
	DirectionalLight gDirectionalLight[5];  //maxumun supported directional light count is 5
	int gDirectionalLightCount;             //current active directional light count

	PointLight gPointLight[5];              //maxumun supported point light count is 5
	int gPointLightCount;                   //current active point light count

	SpotLight gSpotLight[5];                //maxumun supported spot light count is 5
	int gSpotLightCount;			//current active spot light count
};

//Shadow map test
static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f/SMAP_SIZE;

float calcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH)
{
	shadowPosH.xyz /= shadowPosH.w;

	float depth = shadowPosH.z;

	const float dx = SMAP_DX;
	const float2 offset[9] = {
		float2(-dx, -dx),  float2(0.0f, -dx),  float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, dx),   float2(0.0f, dx),   float2(dx, dx)
	};

	float percentLit = 0.0f;

	[unroll]
	for(int i = 0; i < 9; ++i) {
		percentLit += shadowMap.SampleCmpLevelZero(samShadow, shadowPosH.xy+offset[i], depth).r;
	}

	return percentLit /= 9.0f;
}

//parameters
//mat:    mesh material
//light:  directional light structure
//normal: vertex normal vector
//toEye:  vector from vertex to camera
void ProcessDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 toEye, 
							 out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient = mat.ambient * light.ambient;
	float3 lightVec = -light.direction.xyz;
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if(diffuseFactor > 0) {
		diffuse = diffuseFactor * mat.diffuse * light.diffuse;
		float3 refVec = reflect(light.direction.xyz, normal);
		float specFactor = pow(max(dot(refVec, toEye), 0.0f), mat.specular.w);
		specular = specFactor * mat.specular * light.specular;
	}
}

//parameters
//mat:    mesh material
//light:  point light structure
//pos:    vertex position in world space
//normal: vertex normal vector
//toEye:  vector from vertex to camera
void ProcessPointLight(Material mat, PointLight light, float3 pos, float3 normal, float3 toEye, 
					   out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//vector from vertex to point light position
	float3 lightVec = light.position.xyz - pos;
	float len = length(lightVec);
	//light source is to far to affect the vertex
	if(len > light.att.w)
		return;

	//normalize light vector
	lightVec /= len;
	ambient = mat.ambient * light.ambient;
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if(diffuseFactor > 0) {
		diffuse = diffuseFactor * mat.diffuse * light.diffuse;
		float3 refVec = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(refVec, toEye), 0.0f), mat.specular.w);
		specular = specFactor * mat.specular * light.specular;
	}

	//compute attenuation : 1.0 / (a0+a1*d+a2*d*d), d is the distane from vertex position to point light position
	float att = 1.0f / dot(light.att.xyz, float3(1.0f, len, len*len));
	diffuse  *= att;
	specular *= att;
}

void processSpotLight(Material mat, SpotLight light, float3 pos, float3 normal, float3 toEye, 
					  out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//vector from vertex to point light position
	float3 lightVec = light.position.xyz - pos;
	float len = length(lightVec);
	//light source is to far to affect the vertex
	if(len > light.att.w)
		return;

	//normalize light vector
	lightVec /= len;
	ambient = mat.ambient * light.ambient;
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if(diffuseFactor > 0) {
		diffuse = diffuseFactor * mat.diffuse * light.diffuse;
		float3 refVec = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(refVec, toEye), 0.0f), mat.specular.w);
		specular = specFactor * mat.specular * light.specular;
	}

	//compute spot light cone factor:pow(max(dot(-L, D), 0), s)
	//L is the vector from light source to vertex
	//D is the spot light direction
	//s is the exponent to control the size of cone
	float spot = pow(max(dot(-lightVec, light.spotDirection.xyz), 0.0f), light.spotDirection.w);
	//compute attenuation : 1.0 / (a0+a1*d+a2*d*d), d is the distane from vertex position to point light position
	float att = spot / dot(light.att, float4(1.0f, len, len*len, 0));

	ambient *= spot;
	diffuse  *= att;
	specular *= att;
}

void processLight(Material mat, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;
	int i;
	
	[unroll]
	for(i = 0; i < gDirectionalLightCount; ++i) {
		ProcessDirectionalLight(mat, gDirectionalLight[i], normal, toEye, A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
	
	[unroll]
	for(i = 0; i < gPointLightCount; ++i) {
		ProcessPointLight(mat, gPointLight[i], pos, normal, toEye, A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
	
	[unroll]
	for(i = 0; i < gSpotLightCount; ++i) {
		processSpotLight(mat, gSpotLight[i], pos, normal, toEye, A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
}

void processLightShadow(Material mat, float3 pos, float3 normal, float3 toEye, 
			SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH,
			out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;
	int i;
	float dShadow = 1.0f;
	
	[unroll]
	for(i = 0; i < gDirectionalLightCount; ++i) {
		ProcessDirectionalLight(mat, gDirectionalLight[i], normal, toEye, A, D, S);
		dShadow = 1.0f;

		[flatten]
		if(gDirectionalLight[i].lightAttr.x > 0) {
			dShadow = calcShadowFactor(samShadow, shadowMap, shadowPosH);
		}
		
		ambient  += A;
		diffuse  += dShadow*D;
		specular += dShadow*S;
	}
	
	[unroll]
	for(i = 0; i < gPointLightCount; ++i) {
		ProcessPointLight(mat, gPointLight[i], pos, normal, toEye, A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
	
	[unroll]
	for(i = 0; i < gSpotLightCount; ++i) {
		processSpotLight(mat, gSpotLight[i], pos, normal, toEye, A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
}

float4 processReflection(Material mat, TextureCube cubemap, float3 normal, float3 toEye)
{
	float3 incidentDir = -toEye;
	float3 reflectionDir = reflect(incidentDir, normal);
	float4 reflectionColor = cubemap.Sample(samAnisotropic, reflectionDir);
	return mat.reflect*reflectionColor;
}

//transform normal map sample to wprld space
float3 processNormalMapToWorld(float3 normalMapVector, float3 normalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f*normalMapVector - 1.0f;

	float3 N = normalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);
	return bumpedNormalW;
}
