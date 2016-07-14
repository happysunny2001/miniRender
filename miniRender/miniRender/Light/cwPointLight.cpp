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

#include "cwPointLight.h"

NS_MINIR_BEGIN

//cwPointLight* cwPointLight::create(
//const cwVector4D& position,
//const cwVector4D& ambient,
//const cwVector4D& diffuse,
//const cwVector4D& specular,
//const cwVector4D& attenuation)
//{
//	cwPointLight* pLight = new cwPointLight();
//	if (pLight && pLight->init(position, ambient, diffuse, specular, attenuation)) {
//		pLight->autorelease();
//		return pLight;
//	}
//
//	CW_SAFE_DELETE(pLight);
//	return nullptr;
//}

cwPointLight* cwPointLight::create(
	const cwVector3D& pos,
	const cwVector3D& color,
	CWFLOAT radiusBegin,
	CWFLOAT radiusEnd)
{
	cwPointLight* pLight = new cwPointLight();
	if (pLight && pLight->init(pos, color, radiusBegin, radiusEnd)) {
		pLight->autorelease();
		return pLight;
	}

	CW_SAFE_DELETE(pLight);
	return nullptr;
}

cwPointLight::cwPointLight()
{

}

cwPointLight::~cwPointLight()
{

}

//CWBOOL cwPointLight::init(
//	const cwVector4D& position,
//	const cwVector4D& ambient,
//	const cwVector4D& diffuse,
//	const cwVector4D& specular,
//	const cwVector4D& attenuation)
//{
//	this->setPosition(position);
//	this->setAmbient(ambient);
//	this->setDiffuse(diffuse);
//	this->setSpecular(specular);
//	this->setAttenuation(attenuation);
//	this->setRange(attenuation.w);
//
//	return CWTRUE;
//}

CWBOOL cwPointLight::init(
	const cwVector3D& pos,
	const cwVector3D& color,
	CWFLOAT radiusBegin,
	CWFLOAT radiusEnd)
{
	m_nPointLight.m_nPosition = pos;
	m_nPointLight.m_nColor = color;
	m_nPointLight.m_fRadiusBegin = radiusBegin;
	m_nPointLight.m_fRadiusEnd = radiusEnd;

	return CWTRUE;
}

CWUINT cwPointLight::size() const
{
	return sizeof(PointLight_Struct);
}

CWVOID* cwPointLight::data() const
{
	return (CWVOID*)&(this->m_nPointLight);
}

NS_MINIR_END
