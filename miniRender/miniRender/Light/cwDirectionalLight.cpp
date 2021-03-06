﻿/*
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

#include "cwDirectionalLight.h"

NS_MINIR_BEGIN

cwDirectionalLight* cwDirectionalLight::create(
	const cwVector4D& direction,
	const cwVector3D& color)
{
	cwDirectionalLight* pLight = new cwDirectionalLight();
	if (pLight && pLight->init(direction, color)) {
		pLight->autorelease();
		return pLight;
	}

	CW_SAFE_DELETE(pLight);
	return nullptr;
}

//cwDirectionalLight* cwDirectionalLight::create(
//const cwVector4D& direction,
//		const cwVector4D& ambient,
//		const cwVector4D& diffuse,
//		const cwVector4D& specular)
//{
//	cwDirectionalLight* pLight = new cwDirectionalLight();
//	if (pLight && pLight->init(direction, ambient, diffuse, specular)) {
//		pLight->autorelease();
//		return pLight;
//	}
//
//	CW_SAFE_DELETE(pLight);
//	return nullptr;
//}

cwDirectionalLight::cwDirectionalLight()
{

}

cwDirectionalLight::cwDirectionalLight(cwDirectionalLight& light)
{
	m_nDirectionalLight = light.m_nDirectionalLight;
}

cwDirectionalLight::~cwDirectionalLight()
{
	
}

CWBOOL cwDirectionalLight::init(
	const cwVector4D& direction,
	const cwVector3D& color)
{
	m_nDirectionalLight.m_nDirection = direction;
	m_nDirectionalLight.m_nDirection.w = 0;
	m_nDirectionalLight.m_nColor = color;
	m_nDirectionalLight.m_fAttr = 0;

	return CWTRUE;
}

//CWBOOL cwDirectionalLight::init(
//	const cwVector4D& direction,
//	const cwVector4D& ambient,
//	const cwVector4D& diffuse,
//	const cwVector4D& specular)
//{
//	this->setDirection(direction);
//	this->setAmbient(ambient);
//	this->setDiffuse(diffuse);
//	this->setSpecular(specular);
//
//	m_nLightAttr.set(0, 0, 0, 0);
//
//	return CWTRUE;
//}

CWUINT cwDirectionalLight::size() const
{
	return sizeof(m_nDirectionalLight);
}

CWVOID* cwDirectionalLight::data() const
{
	return (CWVOID*)&(this->m_nDirectionalLight);
}

CWVOID cwDirectionalLight::setCastShadow(CWBOOL b)
{
	if (b) {
		m_nDirectionalLight.m_fAttr = 1.0f;
	}
	else {
		m_nDirectionalLight.m_fAttr = 0.0f;
	}
}

CWBOOL cwDirectionalLight::getCastShadow() const
{
	if (m_nDirectionalLight.m_fAttr == 0) return CWFALSE;
	return CWTRUE;
}

NS_MINIR_END
