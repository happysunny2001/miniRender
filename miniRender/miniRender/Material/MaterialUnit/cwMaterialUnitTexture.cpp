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

#include "cwMaterialUnitTexture.h"
#include "Repertory/cwRepertory.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderConstant.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwMaterialUnitTexture* cwMaterialUnitTexture::create(const CWSTRING& strTexture, const CWSTRING& strParamName)
{
	cwMaterialUnitTexture* pMUTexture = new cwMaterialUnitTexture();
	if (pMUTexture && pMUTexture->init(strTexture, strParamName)) {
		pMUTexture->autorelease();
		return pMUTexture;
	}

	CW_SAFE_DELETE(pMUTexture);
	return nullptr;
}

cwMaterialUnitTexture* cwMaterialUnitTexture::createCube(const CWSTRING& strTexture, const CWSTRING& strParamName)
{
	cwMaterialUnitTexture* pMUTexture = new cwMaterialUnitTexture();
	if (pMUTexture && pMUTexture->initCube(strTexture, strParamName)) {
		pMUTexture->autorelease();
		return pMUTexture;
	}

	CW_SAFE_DELETE(pMUTexture);
	return nullptr;
}

cwMaterialUnitTexture::cwMaterialUnitTexture():
m_pTexture(nullptr)
{

}

cwMaterialUnitTexture::~cwMaterialUnitTexture()
{
	CW_SAFE_RELEASE_NULL(m_pTexture);
}

CWBOOL cwMaterialUnitTexture::init(const CWSTRING& strTexture, const CWSTRING& strParamName)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_pTexture = cwRepertory::getInstance().getTextureManager()->getTexture(strTexture);
	if (!m_pTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pTexture);

	m_nStrShaderParam = strParamName;

	return CWTRUE;
}

CWBOOL cwMaterialUnitTexture::initCube(const CWSTRING& strTexture, const CWSTRING& strParamName)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_pTexture = cwRepertory::getInstance().getTextureManager()->getCubeTexture(strTexture);
	if (!m_pTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pTexture);

	m_nStrShaderParam = strParamName;

	return CWTRUE;
}

CWVOID cwMaterialUnitTexture::setTexture(cwTexture* pTexture)
{
	if (m_pTexture == pTexture) return;
	CW_SAFE_RETAIN(pTexture);
	CW_SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;
}

CWVOID cwMaterialUnitTexture::config(cwEffect* pEffect)
{
	cwShader* pShader = pEffect->getShader();
	if (pShader && m_pTexture && !m_nStrShaderParam.empty()) {
		pShader->setVariableTexture(m_nStrShaderParam, m_pTexture);
	}
}

NS_MINIR_END
