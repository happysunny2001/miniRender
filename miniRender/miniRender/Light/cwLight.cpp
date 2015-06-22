/*
Copyright © 2015 Ziwei Wang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "cwLight.h"

NS_CW_BEGIN

cwLight* cwLight::createDirectionalLight(
const cwVector3D& direction,
const cwVector4D& ambient,
const cwVector4D& diffuse,
const cwVector4D& specular)
{
	cwLight* pLight = new cwLight();
	if (pLight && pLight->initDirectionalLight(direction, ambient, diffuse, specular)) {
		pLight->autorelease();
		return pLight;
	}

	CW_SAFE_DELETE(pLight);
	return nullptr;
}

cwLight* cwLight::createPointLight(
	const cwVector3D& position,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector3D& attenuation,
	CWFLOAT range
	)
{
	cwLight* pLight = new cwLight();
	if (pLight && pLight->initPointLight(position, ambient, diffuse, specular, attenuation, range)) {
		pLight->autorelease();
		return pLight;
	}

	CW_SAFE_DELETE(pLight);
	return nullptr;
}

cwLight* cwLight::createSpotLight(
	const cwVector3D& position,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector3D& attenuation,
	CWFLOAT range,
	const cwVector3D& direction,
	CWFLOAT spot
	)
{
	cwLight* pLight = new cwLight();
	if (pLight && pLight->initSpotLight(position, ambient, diffuse, specular, attenuation, range, direction, spot)) {
		pLight->autorelease();
		return pLight;
	}

	CW_SAFE_DELETE(pLight);
	return nullptr;
}

cwLight::cwLight():
m_nIndex(0),
m_iTag(0),
m_strName("")
{

}

cwLight::~cwLight()
{

}

bool cwLight::initDirectionalLight(
	const cwVector3D& direction,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular)
{
	this->setDirection(direction);
	this->setAmbient(ambient);
	this->setDiffuse(diffuse);
	this->setSpecular(specular);
	this->setType(eLightTypeDirectional);
	return true;
}

bool cwLight::initPointLight(
	const cwVector3D& position,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector3D& attenuation,
	CWFLOAT range)
{
	this->setPosition(position);
	this->setAmbient(ambient);
	this->setDiffuse(diffuse);
	this->setSpecular(specular);
	this->setAttenuation(attenuation);
	this->setRange(range);
	this->setType(eLightTypePoint);
	return true;
}

bool cwLight::initSpotLight(
	const cwVector3D& position,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector3D& attenuation,
	CWFLOAT range,
	const cwVector3D& direction,
	CWFLOAT spot)
{
	this->setPosition(position);
	this->setAmbient(ambient);
	this->setDiffuse(diffuse);
	this->setSpecular(specular);
	this->setAttenuation(attenuation);
	this->setRange(range);
	this->setSpotDirection(direction);
	this->setSpotLightExponent(spot);
	this->setType(eLightTypeSpot);
	return true;
}

void cwLight::setPosition(const cwVector3D& pos)
{
	this->position = pos;
}

const cwVector3D& cwLight::getPosition()
{
	return this->position;
}

void cwLight::setDirection(const cwVector3D& dir)
{
	this->direction.x = dir.x;
	this->direction.y = dir.y;
	this->direction.z = dir.z;
}

const cwVector3D& cwLight::getDirection()
{
	return this->direction;
}

void cwLight::setAttenuation(const cwVector3D& att)
{
	this->att.x = att.x;
	this->att.y = att.y;
	this->att.z = att.z;
}

const cwVector3D cwLight::getAttenuation()
{
	return cwVector3D(this->att.x, this->att.y, this->att.z);
}

void cwLight::setRange(CWFLOAT f)
{
	this->att.w = f;
}

CWFLOAT cwLight::getRange()
{
	return this->att.w;
}

void cwLight::setSpotLightExponent(CWFLOAT f)
{
	this->spotDirection.w = f;
}

CWFLOAT cwLight::getSpotLightExponent()
{
	return this->spotDirection.w;
}

void cwLight::setSpotDirection(const cwVector3D& dir)
{
	this->spotDirection.x = dir.x;
	this->spotDirection.y = dir.y;
	this->spotDirection.z = dir.z;
}

const cwVector3D cwLight::getSpotDirection()
{
	return cwVector3D(this->spotDirection.x, this->spotDirection.y, this->spotDirection.z);
}

void cwLight::setAmbient(const cwVector4D& color)
{
	this->ambient = color;
}

const cwVector4D& cwLight::getAmbient()
{
	return this->ambient;
}

void cwLight::setDiffuse(const cwVector4D& color)
{
	this->diffuse = color;
}

const cwVector4D& cwLight::getDiffuse()
{
	return this->diffuse;
}

void cwLight::setSpecular(const cwVector4D& color)
{
	this->specular = color;
}

const cwVector4D& cwLight::getSpecular()
{
	return this->specular;
}

const CWUINT cwLight::size() const
{
	return sizeof(cwVector4D)* 6;
}

CWVOID* cwLight::data() const 
{
	return (CWVOID*)&(this->direction);
}

void cwLight::setType(eLightType type)
{
	this->type = (CWFLOAT)type;
}

const eLightType cwLight::getType() const
{
	return (eLightType)(int)(this->type);
}

NS_CW_END