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
#include "Entity/cwPrimitiveEntity.h"
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
m_pCurrRenderStage(nullptr),
m_pPrimitiveEntity(nullptr),
m_pPrimitiveBatch(nullptr)
{

}

cwRenderer::~cwRenderer()
{
	m_nVecStage.clear();
	CW_SAFE_RELEASE_NULL(m_pPrimitiveEntity);
	CW_SAFE_DELETE(m_pPrimitiveBatch);

	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;
	m_pRenderListHead = nullptr;
	m_pCurrRenderStage = nullptr;
}

CWBOOL cwRenderer::init()
{
	buildPrimitiveEntity();

	return true;
}

CWVOID cwRenderer::buildPrimitiveEntity()
{
	CW_SAFE_RELEASE_NULL(m_pPrimitiveEntity);
	m_pPrimitiveEntity = cwPrimitiveEntity::create();
	CW_SAFE_RETAIN(m_pPrimitiveEntity);

	CW_SAFE_DELETE(m_pPrimitiveBatch);
	m_pPrimitiveBatch = new cwRenderBatch();
	m_pPrimitiveBatch->m_pEffect = m_pPrimitiveEntity->getEffect();
	m_pPrimitiveBatch->m_pBlend = m_pPrimitiveEntity->getBlend();
	m_pPrimitiveBatch->m_pStencil = m_pPrimitiveEntity->getStencil();
	m_pPrimitiveBatch->m_pEntity = m_pPrimitiveEntity;
	m_pPrimitiveBatch->m_nWorldTrans = cwMatrix4X4::identityMatrix;
}

CWVOID cwRenderer::renderPrimitiveEntity()
{
	if (m_pPrimitiveBatch)
		this->render(m_pPrimitiveBatch);
}

CWVOID cwRenderer::renderPrimitive(const cwAABB& aabb)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(aabb);
	}
}

CWVOID cwRenderer::renderPrimitive(const cwAABB& aabb, const cwVector4D& color)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(aabb, color);
	}
}

CWVOID cwRenderer::setCurrCamera(cwCamera* pCamera)
{
	m_pCurrCamera = pCamera;
}

CWVOID cwRenderer::setCurrShader(cwShader* pShader)
{
	if (m_pCurrShader == pShader) return;
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

	renderPrimitiveEntity();
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
	if (!pBatch->m_pEffect) return;

	pBatch->m_pEffect->render(pBatch);
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

	configDirectionalLight();
	configPointLight();
	configSpotLight();
}

CWVOID cwRenderer::configDirectionalLight()
{
	if (!m_pCurrShader->hasVariable(eShaderParamDirectionalLight) || 
		!m_pCurrShader->hasVariable(eShaderParamDirectionalLightCnt)) return;

	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwVector<cwDirectionalLight*>& vecLight = pCurrScene->getDirectionalLights();
	if (vecLight.empty()) {
		m_pCurrShader->setVariableInt(eShaderParamDirectionalLightCnt, 0);
		return;
	}

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamDirectionalLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamDirectionalLightCnt, (CWINT)(vecLight.size()));
}

CWVOID cwRenderer::configPointLight()
{
	if (!m_pCurrShader->hasVariable(eShaderParamPointLight) ||
		!m_pCurrShader->hasVariable(eShaderParamPointLightCnt)) return;

	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwVector<cwPointLight*>& vecLight = pCurrScene->getPointLights();
	if (vecLight.empty()) {
		m_pCurrShader->setVariableInt(eShaderParamPointLightCnt, 0);
		return;
	}

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamPointLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamPointLightCnt, (CWINT)(vecLight.size()));
}

CWVOID cwRenderer::configSpotLight()
{
	if (!m_pCurrShader->hasVariable(eShaderParamSpotLight) ||
		!m_pCurrShader->hasVariable(eShaderParamSpotLightCnt)) return;

	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwVector<cwSpotLight*>& vecLight = pCurrScene->getSpotLights();
	if (vecLight.empty()) {
		m_pCurrShader->setVariableInt(eShaderParamSpotLightCnt, 0);
		return;
	}

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamSpotLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamSpotLightCnt, (CWINT)(vecLight.size()));
}

NS_MINIR_END
