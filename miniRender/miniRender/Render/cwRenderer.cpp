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

#include "utlist.h"

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
m_pCurrShader(nullptr),
m_iListPoolIndex(0),
m_pRenderListHead(nullptr),
m_pCurrRenderStage(nullptr)
{

}

cwRenderer::~cwRenderer()
{
	m_nVecStage.clear();

	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;
	m_pRenderListHead = nullptr;
	m_pCurrRenderStage = nullptr;
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
	m_nVecStage.pushBack(pStage);
}

CWVOID cwRenderer::addStageRealTime(cwStage* pStage)
{
	if (!pStage) return;
	if (!m_pCurrRenderStage) return;

	cwRenderer::sRendererListNode* pNode = nullptr;
	DL_SEARCH_SCALAR(m_pRenderListHead, pNode, m_pStage, m_pCurrRenderStage);
	if (pNode) {
		sRendererListNode* pNewNode = getAvaiableListNode();
		if (pNewNode) {
			pNewNode->m_pStage = pStage;

			if (pNode->next)
				DL_PREPEND_ELEM(m_pRenderListHead, pNode->next, pNewNode);
			else
				DL_APPEND(m_pRenderListHead, pNewNode);
		}
	}
}

CWVOID cwRenderer::begin()
{
	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;
	m_pCurrRenderStage = nullptr;

	m_pRenderListHead = buildStageList();
}

CWVOID cwRenderer::render()
{
	if (m_pRenderListHead) {
		sRendererListNode* pElement = nullptr;

		DL_FOREACH(m_pRenderListHead, pElement) {
			m_pCurrRenderStage = pElement->m_pStage;
			this->render(m_pCurrRenderStage);
		}
	}
}

CWVOID cwRenderer::end()
{
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

	pDevice->setBlend(pBatch->m_pBlend);
	pDevice->setStencil(pBatch->m_pStencil);
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

cwRenderer::sRendererListNode* cwRenderer::getAvaiableListNode()
{
	if (m_iListPoolIndex >= CW_RENDERER_LIST_POOL_SIZE) return nullptr;
	return &(m_nListNodePool[m_iListPoolIndex++]);
}

cwRenderer::sRendererListNode* cwRenderer::buildStageList()
{
	m_iListPoolIndex = 0;
	sRendererListNode* pHead = nullptr;
	for (auto pStage : m_nVecStage) {
		sRendererListNode* pNode = getAvaiableListNode();
		if (pNode) {
			pNode->m_pStage = pStage;
			DL_APPEND(pHead, pNode);
		}
	}

	return pHead;
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
