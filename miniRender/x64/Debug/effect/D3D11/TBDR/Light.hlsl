#ifndef __LIGHT_HLSL__
#define __LIGHT_HLSL__

#include "TBDRDefines.hlsl"
#include "SurfaceData.hlsl"
#include "PerFrameConstant.hlsl"

struct PointLight
{
	float3 position;
	float  radiusBegin;
	float3 color;
	float  radiusEnd;
};

struct DirectionalLight
{
    float4 direction; //w:unused
    float3 color;
    float attr;       //shadow flag,0:not cast shadow,1:cast shadow
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

StructuredBuffer<PointLight> gPointLights;
uint gPointLightCnt;

DirectionalLight gDirectionalLight;
uint gEnableDirectionalLight;
Texture2D gShadowMapTexture;

float linstep(float min, float max, float v)
{
    return saturate((v - min) / (max - min));
}

void AccumulatePhongBRDF(float3 normal,
                         float3 lightDir,
                         float3 viewDir,
                         float3 lightContrib,
                         float specularPower,
                         inout float3 litDiffuse,
                         inout float3 litSpecular)
{
    // Simple Phong
    float NdotL = dot(normal, lightDir);
    [flatten] if (NdotL > 0.0f) {
        float3 r = reflect(lightDir, normal);
        float RdotV = max(0.0f, dot(r, viewDir));
        float specular = pow(RdotV, specularPower);

        litDiffuse += lightContrib * NdotL;
        litSpecular += lightContrib * specular;
    }
}

void PointLightAccumulate(PointLight light, SurfaceData surface, inout float3 lit)
{
	float3 directionToLight = light.position - surface.positionView.xyz;
    float distanceToLight = length(directionToLight);

    [branch] if (distanceToLight < light.radiusEnd) {
    	float attenuation = linstep(light.radiusEnd, light.radiusBegin, distanceToLight);
    	directionToLight *= rcp(distanceToLight);

    	float3 litDiffuse = float3(0.0f, 0.0f, 0.0f);
        float3 litSpecular = float3(0.0f, 0.0f, 0.0f);

        AccumulatePhongBRDF(surface.normalView, directionToLight, normalize(surface.positionView.xyz),
            attenuation * light.color.rgb, surface.specular.w, litDiffuse, litSpecular);

        lit += surface.diffuse.rgb * (litDiffuse + surface.specular.r * litSpecular)*surface.fAOFactor;
    }
}

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f/SMAP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH)
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

void DirectionalLightAccumulate(DirectionalLight light, SurfaceData surface, inout float3 lit)
{
    float3 lightVec = -light.direction.xyz;
    float diffuseFactor = dot(lightVec, surface.normalView);

    if(diffuseFactor > 0) {
        float fShafowFactor = 1.0f;
        if(light.attr > 0) {
            float4 posShadowH = mul(surface.positionWorld, gMatShadowTransform);
            fShafowFactor = CalcShadowFactor(samShadow, gShadowMapTexture, posShadowH);
        }

        lit += surface.diffuse.rgb*diffuseFactor*light.color*fShafowFactor*surface.fAOFactor;
    }
}

#endif
