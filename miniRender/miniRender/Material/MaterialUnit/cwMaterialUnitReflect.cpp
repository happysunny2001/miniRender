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

#include "cwMaterialUnitReflect.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShader.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwMaterialUnitReflect* cwMaterialUnitReflect::create(const CWSTRING& strTexture, CWFLOAT fFactor)
{
	cwMaterialUnitReflect* pMUReflect = new cwMaterialUnitReflect();
	if (pMUReflect && pMUReflect->init(strTexture, fFactor)) {
		pMUReflect->autorelease();
		return pMUReflect;
	}

	CW_SAFE_DELETE(pMUReflect);
	return nullptr;
}

cwMaterialUnitReflect* cwMaterialUnitReflect::create()
{
	cwMaterialUnitReflect* pMUReflect = new cwMaterialUnitReflect();
	if (pMUReflect && pMUReflect->init(0.5f)) {
		pMUReflect->autorelease();
		return pMUReflect;
	}

	CW_SAFE_DELETE(pMUReflect);
	return nullptr;
}

cwMaterialUnitReflect::cwMaterialUnitReflect():
m_pReflectTexture(nullptr),
m_fReflectFactor(0.5f)
{

}

cwMaterialUnitReflect::~cwMaterialUnitReflect()
{
	CW_SAFE_RELEASE_NULL(m_pReflectTexture);
}

CWBOOL cwMaterialUnitReflect::init(const CWSTRING& strTexture, CWFLOAT fFactor)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_pReflectTexture = cwRepertory::getInstance().getTextureManager()->getCubeTexture(strTexture);
	if (!m_pReflectTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pReflectTexture);

	m_fReflectFactor = fFactor;

	return CWTRUE;
}

CWBOOL cwMaterialUnitReflect::init(CWFLOAT fFactor)
{
	if (!cwMaterialUnit::init()) return CWFALSE;

	m_fReflectFactor = fFactor;

	return CWTRUE;
}

CWVOID cwMaterialUnitReflect::config(cwEffect* pEffect)
{
	cwShader* pShader = pEffect->getShader();
	if (pShader) {
		if (m_pReflectTexture) {
			pShader->setVariableTexture(eShaderParamReflectCubeMap, m_pReflectTexture);
		}

		pShader->setVariableFloat(eShaderParamReflectFactor, m_fReflectFactor);
	}
}

NS_MINIR_END
