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

#include <assert.h>

NS_MINI_BEGIN

cwMaterial* cwMaterial::create(
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector4D& reflect,
	const string& strShader)
{
	cwMaterial* pMaterial = new cwMaterial();
	if (pMaterial && pMaterial->init(ambient, diffuse, specular, reflect, strShader)) {
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
m_pShader(nullptr),
m_pDiffuseTexture(nullptr),
m_pBlendOp(nullptr)
{
	m_nMatData.m_nAmbient  = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMatData.m_nDiffuse  = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMatData.m_nSpecular = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);
	m_nMatData.m_nReflect  = cwVector4D(1.0f, 1.0f, 1.0f, 1.0f);

}

cwMaterial::~cwMaterial()
{
	CW_SAFE_RELEASE_NULL(m_pShader);
	CW_SAFE_RELEASE_NULL(m_pBlendOp);
	CW_SAFE_RELEASE_NULL(m_pDiffuseTexture);
}

bool cwMaterial::init(
	const cwVector4D& ambient,
	const cwVector4D& diffuse,
	const cwVector4D& specular,
	const cwVector4D& reflect,
	const string& strShader)
{
	this->m_nMatData.m_nAmbient = ambient;
	this->m_nMatData.m_nDiffuse = diffuse;
	this->m_nMatData.m_nSpecular = specular;
	this->m_nMatData.m_nReflect = reflect;
	this->setShader(strShader);
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

void cwMaterial::setShader(const string& strShader)
{
	m_pShader = cwRepertory::getInstance().getShaderManager()->getShader(strShader);
	assert(m_pShader != nullptr);
	CW_SAFE_RETAIN(m_pShader);
}

void cwMaterial::setDiffuseTexture(cwTexture* pTexture)
{
	CW_SAFE_RELEASE_NULL(m_pDiffuseTexture);
	m_pDiffuseTexture = pTexture;
	CW_SAFE_RETAIN(m_pDiffuseTexture);
}

void cwMaterial::setDiffuseTexture(const string& strTexName)
{
	cwTexture* pTexture = cwTextureManager::getInstance().getTexture(strTexName);
	CW_SAFE_RELEASE_NULL(m_pDiffuseTexture);
	m_pDiffuseTexture = pTexture;
	CW_SAFE_RETAIN(m_pDiffuseTexture);
}

void cwMaterial::setBlend(cwBlend* pBlendOp)
{
	CW_SAFE_RELEASE_NULL(m_pBlendOp);
	m_pBlendOp = pBlendOp;
	CW_SAFE_RETAIN(m_pBlendOp);
}

void cwMaterial::configEffect()
{
	if (!m_pShader) return;

	m_pShader->setVariableData(CW_SHADER_MATERIAL, this->getColorData(), 0, this->getColorDataSize());

	if (m_pShader->hasVariable(CW_SHADER_DIFFUSE_TEXTURE)) {
		m_pShader->setVariableTexture(CW_SHADER_DIFFUSE_TEXTURE, this->getDiffuseTexture());
	}

	cwRepertory::getInstance().getDevice()->setBlend(this->getBlend());
}

NS_MINI_END