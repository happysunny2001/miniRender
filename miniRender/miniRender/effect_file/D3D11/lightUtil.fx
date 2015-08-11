#include "material.fx"

//Light Util
//support directional light, point light, spot light

//material structure
//struct Material
//{
//	float4 ambient;
//	float4 diffuse;
//	float4 specular; //w:Spec Power
//	float4 reflect;
//};

//directional light structure
struct DirectionalLight
{
	float3 direction;
	float type;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 pad1;
	float4 pad2;
};

//point light structure
struct PointLight
{
	float3 position;
	float type;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 att; //x,y,z:attenuation A0, A1, A2; w:range
	float4 pad;
};

//spot light structure
struct SpotLight
{
	float3 position;
	float type;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 att; //x,y,z:attenuation A0, A1, A2; w:range
	float4 spotDirection; //x, y, z:spot light position; w:spot
};

struct TypelessLight
{
	float3 pad0;
	float type;
	float4 pad1;
	float4 pad2;
	float4 pad3;
	float4 pad4;
	float4 pad5;
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
	float3 lightVec = -light.direction;
	float diffuseFactor = dot(lightVec, normal);

	if(diffuseFactor > 0) {
		diffuse = diffuseFactor * mat.diffuse * light.diffuse;
		float3 refVec = reflect(light.direction, normal);
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
	float3 lightVec = light.position - pos;
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
	float3 lightVec = light.position - pos;
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