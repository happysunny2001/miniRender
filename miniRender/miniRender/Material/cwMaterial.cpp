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

#include "Material/cwMaterial.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShaderManager.h"
#include "Shader/cwShaderConstant.h"
#include "Shader/cwShader.h"
#include "Blend/cwBlend.h"
#include "Device/cwDevice.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Effect/cwEffect.h"

#include <assert.h>

NS_MINIR_BEGIN

cwMaterial* cwMaterial::create()
{
	cwMaterial* pMaterial = new cwMaterial();
	if (pMaterial && pMaterial->init()) {
		pMaterial->autorelease();
		return pMaterial;
	}

	CW_SAFE_DELETE(pMaterial);
	return nullptr;
}

cwMaterial* cwMaterial::create(
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector4D& reflect)
{
	cwMaterial* pMaterial = new cwMaterial();
	if (pMaterial && pMaterial->init(ambient, diffuse, specular, reflect)) {
		pMaterial->autorelease();
		return pMaterial;
	}

	CW_SAFE_DELETE(pMaterial);
	return nullptr;
}

cwMaterial::cwMaterial():
m_pDiffuseTexture(nullptr)
{
	m_nMatData.m_nAmbient  = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMatData.m_nDiffuse  = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMatData.m_nSpecular = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMatData.m_nReflect  = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);

	m_nDiffTextureTrans = cwVector2D(0.0f, 0.0f);
	m_nDiffTextureScale = cwVector2D(1.0f, 1.0f);

}

cwMaterial::~cwMaterial()
{
	CW_SAFE_RELEASE_NULL(m_pDiffuseTexture);
}

bool cwMaterial::init()
{
	return true;
}

bool cwMaterial::init(
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector4D& reflect)
{
	this->m_nMatData.m_nAmbient = ambient;
	this->m_nMatData.m_nDiffuse = diffuse;
	this->m_nMatData.m_nSpecular = specular;
	this->m_nMatData.m_nReflect = reflect;
	return true;
}

void cwMaterial::setAmbient(const cwVector4D& color)
{
	m_nMatData.m_nAmbient = color;
}

void cwMaterial::setDiffuse(const cwVector4D& color)
{
	m_nMatData.m_nDiffuse = color;
}

void cwMaterial::setSpecular(const cwVector4D& color)
{
	m_nMatData.m_nSpecular = color;
}

void cwMaterial::setReflect(const cwVector4D& color)
{
	m_nMatData.m_nReflect = color;
}

void cwMaterial::setDiffuseTexture(cwTexture* pTexture)
{
	if (m_pDiffuseTexture == pTexture) return;
	CW_SAFE_RELEASE_NULL(m_pDiffuseTexture);
	m_pDiffuseTexture = pTexture;
	CW_SAFE_RETAIN(m_pDiffuseTexture);
}

void cwMaterial::setDiffuseTexture(const string& strTexName)
{
	cwTexture* pTexture = cwRepertory::getInstance().getTextureManager()->getTexture(strTexName);
	if (m_pDiffuseTexture == pTexture) return;
	CW_SAFE_RELEASE_NULL(m_pDiffuseTexture);
	m_pDiffuseTexture = pTexture;
	CW_SAFE_RETAIN(m_pDiffuseTexture);
}

void cwMaterial::setDiffuseTextureTrans(const cwMatrix4X4& trans)
{
	m_nDiffuseTrans = trans;
}

void cwMaterial::moveDiffuseTexture(CWFLOAT x, CWFLOAT y)
{
	m_nDiffTextureTrans.x = x;
	m_nDiffTextureTrans.y = y;
	updateDiffuseTexture();
}

void cwMaterial::moveDiffuseTexture(const cwVector2D& dir)
{
	m_nDiffTextureTrans.x = dir.x;
	m_nDiffTextureTrans.y = dir.y;
	updateDiffuseTexture();
}

void cwMaterial::scaleDiffuseTexture(CWFLOAT x, CWFLOAT y)
{
	m_nDiffTextureScale.x = x;
	m_nDiffTextureScale.y = y;
	updateDiffuseTexture();
}

void cwMaterial::scaleDiffuseTexture(const cwVector2D& scale)
{
	m_nDiffTextureScale.x = scale.x;
	m_nDiffTextureScale.y = scale.y;
	updateDiffuseTexture();
}

void cwMaterial::updateDiffuseTexture()
{
	m_nDiffuseTrans.m11 = m_nDiffTextureScale.x; m_nDiffuseTrans.m12 = 0;                     m_nDiffuseTrans.m13 = 0;    m_nDiffuseTrans.m14 = 0;
	m_nDiffuseTrans.m21 = 0;                     m_nDiffuseTrans.m22 = m_nDiffTextureScale.y; m_nDiffuseTrans.m23 = 0;    m_nDiffuseTrans.m24 = 0;
	m_nDiffuseTrans.m31 = 0;                     m_nDiffuseTrans.m32 = 0;                     m_nDiffuseTrans.m33 = 1.0f; m_nDiffuseTrans.m34 = 0;
	m_nDiffuseTrans.m41 = m_nDiffTextureTrans.x; m_nDiffuseTrans.m42 = m_nDiffTextureTrans.y; m_nDiffuseTrans.m43 = 0;    m_nDiffuseTrans.m44 = 1.0f;
}

void cwMaterial::configEffect(cwEffect* pEffect)
{
	if (pEffect) {
		cwShader* pShader = pEffect->getShader();
		if (pShader) {
			configShader(pShader);
		}

		configUnits(pEffect);
	}
}

CWVOID cwMaterial::configShader(cwShader* pShader)
{
	if (pShader) {
		pShader->setVariableData(eShaderParamMaterial, this->getColorData(), 0, this->getColorDataSize());
		pShader->setVariableTexture(eShaderParamDiffuseTexture, this->getDiffuseTexture());
	}
}

CWVOID cwMaterial::configUnits(cwEffect* pEffect)
{
	if (!m_nVecMatUnits.empty()) {
		for (auto pUnit : m_nVecMatUnits) {
			pUnit->config(pEffect);
		}
	}
}

CWVOID cwMaterial::addMaterialUnit(cwMaterialUnit* pMatUnit)
{
	m_nVecMatUnits.pushBack(pMatUnit);
	pMatUnit->setParentMaterial(this);
}

NS_MINIR_END
