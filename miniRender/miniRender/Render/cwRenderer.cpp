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

#include "cwRenderer.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Entity/cwScene.h"
#include "Entity/cwEntity.h"
#include "Engine/cwEngine.h"
#include "Shader/cwShader.h"
#include "Material/cwMaterial.h"
#include "Effect/cwEffect.h"
#include "cwStage.h"
#include "cwRenderBatch.h"

NS_MINIR_BEGIN

cwRenderer* cwRenderer::create()
{
	cwRenderer* pRenderer = new cwRenderer();
	if (pRenderer && pRenderer->init()) {
		pRenderer->autorelease();
		return pRenderer;
	}

	CW_SAFE_DELETE(pRenderer);
	return nullptr;
}

cwRenderer::cwRenderer():
m_pCurrCamera(nullptr),
m_pCurrShader(nullptr)
{

}

cwRenderer::~cwRenderer()
{
	for (auto pStage : m_nVecStage) {
		CW_SAFE_DELETE(pStage);
	}

	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;
}

CWBOOL cwRenderer::init()
{
	return true;
}

CWVOID cwRenderer::setCurrCamera(cwCamera* pCamera)
{
	m_pCurrCamera = pCamera;
}

CWVOID cwRenderer::setCurrShader(cwShader* pShader)
{
	m_pCurrShader = pShader;
	configLight();
}

CWVOID cwRenderer::addStage(cwStage* pStage)
{
	m_nVecStage.push_back(pStage);
}

CWVOID cwRenderer::render()
{
	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;

	for (auto pStage : m_nVecStage) {
		this->render(pStage);
	}

	cwRepertory::getInstance().getDevice()->swap();
}

CWVOID cwRenderer::render(cwStage* pStage)
{
	if (!pStage) return;

	pStage->begin();
	pStage->render();
	pStage->end();
}

CWVOID cwRenderer::render(cwRenderBatch* pBatch)
{
	if (!pBatch) return;
	if (!pBatch->m_pEntity) return;

	cwRenderObject* pRenderObj = pBatch->m_pEntity->getRenderObj();
	if (!pRenderObj) return;

	cwMaterial* pMaterial = pBatch->m_pEntity->getMaterial();
	if (pMaterial)
		pMaterial->configShader(m_pCurrShader);

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();

	pDevice->setBlend(pBatch->m_pEntity->getBlend());
	pDevice->setStencil(pBatch->m_pEntity->getStencil());
	pDevice->setShaderWorldTrans(m_pCurrShader, pBatch->m_nWorldTrans, m_pCurrCamera);
	pDevice->draw(m_pCurrShader, pBatch->m_nStrTech, pRenderObj);

	pBatch->m_pEntity->render();
}

cwStage* cwRenderer::getStage(const CWSTRING& strName)
{
	for (auto pStage : m_nVecStage) {
		if (pStage->getName() == strName) return pStage;
	}

	return nullptr;
}

CWVOID cwRenderer::configLight()
{
	if (!m_pCurrShader) return;
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene) return;
	if (pCurrScene->getLights().empty()) return;
	if (!m_pCurrShader->hasVariable(eShaderParamLight) || !m_pCurrShader->hasVariable(eShaderParamLightCnt)) return;

	CWUINT index = 0;
	const cwVector<cwLight*>& vecLight = pCurrScene->getLights();
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamLightCnt, (CWINT)(vecLight.size()));
}

NS_MINIR_END
