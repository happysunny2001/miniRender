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

#include "cwSpotLight.h"

NS_MINIR_BEGIN

cwSpotLight* cwSpotLight::create(
const cwVector4D& position,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector4D& attenuation,
	const cwVector4D& direction)
{
	cwSpotLight* pLight = new cwSpotLight();
	if (pLight && pLight->init(position, ambient, diffuse, specular, attenuation, direction)) {
		pLight->autorelease();
		return pLight;
	}

	CW_SAFE_DELETE(pLight);
	return nullptr;
}

cwSpotLight::cwSpotLight()
{

}

cwSpotLight::~cwSpotLight()
{

}

CWBOOL cwSpotLight::init(
	const cwVector4D& position,
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector4D& attenuation,
	const cwVector4D& direction)
{
	this->setPosition(position);
	this->setAmbient(ambient);
	this->setDiffuse(diffuse);
	this->setSpecular(specular);
	this->setAttenuation(attenuation);
	this->setRange(attenuation.w);
	this->setSpotDirection(direction);
	this->setSpotLightExponent(direction.w);

	return CWTRUE;
}

NS_MINIR_END
