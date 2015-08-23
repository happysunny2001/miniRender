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

cbuffer cbPerFrame
{
	DirectionalLight gDirectionalLight[2];  //maxumun supported directional light count is 2
	int gDirectionalLightCount;             //current active directional light count

	PointLight gPointLight[5];              //maxumun supported point light count is 5
	int gPointLightCount;                   //current active point light count

	SpotLight gSpotLight[3];                //maxumun supported spot light count is 3
	int gSpotLightCount;			//current active spot light count
};

cbuffer cbPerFrame
{
	float3 gEyePosWorld;	      //camera position in world space
};

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