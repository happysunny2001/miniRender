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

#include "cwMaterialUnitDynamicReflect.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderConstant.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwMaterialUnitDynamicReflect* cwMaterialUnitDynamicReflect::create(const CWSTRING& strTexture, CWFLOAT fFactor)
{
	cwMaterialUnitDynamicReflect* pMUReflect = new cwMaterialUnitDynamicReflect();
	if (pMUReflect && pMUReflect->init(strTexture, fFactor)) {
		pMUReflect->autorelease();
		return pMUReflect;
	}

	CW_SAFE_DELETE(pMUReflect);
	return nullptr;
}

cwMaterialUnitDynamicReflect* cwMaterialUnitDynamicReflect::create()
{
	cwMaterialUnitDynamicReflect* pMUReflect = new cwMaterialUnitDynamicReflect();
	if (pMUReflect && pMUReflect->init()) {
		pMUReflect->autorelease();
		return pMUReflect;
	}

	CW_SAFE_DELETE(pMUReflect);
	return nullptr;
}

cwMaterialUnitDynamicReflect::cwMaterialUnitDynamicReflect() :
m_fReflectFactor(0.5f),
m_nStrShaderFactorParam(CW_SHADER_REFLECT_FACTOR)
{

}

cwMaterialUnitDynamicReflect::~cwMaterialUnitDynamicReflect()
{

}

CWBOOL cwMaterialUnitDynamicReflect::init(const CWSTRING& strTexture, CWFLOAT fFactor)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_pTexture = cwRepertory::getInstance().getTextureManager()->getCubeTexture(strTexture);
	if (!m_pTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pTexture);

	m_fReflectFactor = fFactor;
	m_nStrShaderParam = CW_SHADER_REFLECT_CUBE_MAP;

	return CWTRUE;
}

CWBOOL cwMaterialUnitDynamicReflect::init()
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_fReflectFactor = -1.0f;
	m_nStrShaderParam = CW_SHADER_REFLECT_CUBE_MAP;

	return CWTRUE;
}

CWVOID cwMaterialUnitDynamicReflect::config(cwEffect* pEffect)
{
	cwShader* pShader = pEffect->getShader();
	if (pShader) {
		if (m_pTexture && !m_nStrShaderParam.empty()) {
			pShader->setVariableTexture(m_nStrShaderParam, m_pTexture);
		}

		if (m_fReflectFactor > 0.0f && !m_nStrShaderFactorParam.empty())
			pShader->setVariableFloat(m_nStrShaderFactorParam, m_fReflectFactor);
	}
}

NS_MINIR_END
