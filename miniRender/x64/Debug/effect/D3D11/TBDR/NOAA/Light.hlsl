#ifndef __LIGHT_HLSL__
#define __LIGHT_HLSL__

#include "TBDRDefines.hlsl"
#include "SurfaceData.hlsl"

struct PointLight
{
	float3 position;
	float  radiusBegin;
	float3 color;
	float  radiusEnd;
};

StructuredBuffer<PointLight> gPointLights;
uint gPointLightCnt;

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

        lit += surface.diffuse.rgb * (litDiffuse + surface.specular.r * litSpecular);
    }
}

#endif
