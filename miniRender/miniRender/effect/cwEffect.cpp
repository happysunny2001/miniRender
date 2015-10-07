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

#include "cwEffect.h"
#include "Render/cwRenderBatch.h"
#include "Render/cwRenderer.h"
#include "RenderObject/cwRenderObject.h"
#include "Entity/cwEntity.h"
#include "Material/cwMaterial.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"

NS_MINIR_BEGIN

cwEffect* cwEffect::create()
{
	cwEffect* pEffect = new cwEffect();
	if (pEffect) {
		pEffect->autorelease();
		return pEffect;
	}

	CW_SAFE_DELETE(pEffect);
	return nullptr;
}

cwEffect::cwEffect():
m_pShader(nullptr)
{

}

cwEffect::~cwEffect()
{
	CW_SAFE_RELEASE_NULL(m_pShader);
	m_nVecParameter.clear();
}

CWVOID cwEffect::setShader(cwShader* pShader)
{
	if (pShader == m_pShader) return;
	CW_SAFE_RETAIN(pShader);
	CW_SAFE_RELEASE_NULL(m_pShader);
	m_pShader = pShader;
}

CWVOID cwEffect::setTech(const CWSTRING& strTech)
{
	m_strTech = strTech;
}

CWVOID cwEffect::addParameter(cwEffectParameter* pEffectParameter)
{
	if (pEffectParameter) {
		m_nVecParameter.pushBack(pEffectParameter);
	}
}

CWVOID cwEffect::config()
{
	for (auto pParam : m_nVecParameter) {
		pParam->binding(m_pShader);
	}
}

CWVOID cwEffect::render(cwRenderBatch* pBatch)
{
	if (!pBatch) return;
	if (!pBatch->m_pEntity) return;

	//cwRepertory::getInstance().getEngine()->getRenderer()->setCurrShader(m_pShader);
	this->config();

	//cwMaterial* pMaterial = pBatch->m_pEntity->getMaterial();
	//if (pMaterial)
	//	pMaterial->configShader(m_pShader);

	//cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	//cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getRenderer()->getCurrCamera();

	//pDevice->setBlend(pBatch->m_pBlend);
	//pDevice->setStencil(pBatch->m_pStencil);
	//pDevice->setShaderWorldTrans(m_pShader, pBatch->m_nWorldTrans, pCamera);
	pBatch->m_pEntity->render(pBatch);
	//pDevice->draw(m_pShader, m_strTech, pRenderObj);

	//pBatch->m_pEntity->render();
}

NS_MINIR_END

