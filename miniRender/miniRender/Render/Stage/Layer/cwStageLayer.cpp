/*
Copyright © 2015-2016 Ziwei Wang

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

#include "cwStageLayer.h"
#include "Entity/cwEntity.h"
#include "Effect/cwEffect.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "Camera/cwCamera.h"
#include "Render/ProcessingUnit/StageLayer/cwPUStageLayer.h"
#include "Render/ProcessingUnit/Batch/cwPUBatch.h"

NS_MINIR_BEGIN

cwStageLayer* cwStageLayer::create()
{
	cwStageLayer* pStageLayer = new cwStageLayer();
	if (pStageLayer) {
		pStageLayer->autorelease();
		return pStageLayer;
	}

	return nullptr;
}

cwStageLayer::cwStageLayer():
m_eFilterType(eStageLayerFliterEntity),
m_bTransparent(CWFALSE),
m_eRenderState(eRenderStateNone),
m_pCamera(nullptr)
{
	reset();
	m_nLayerType = "Normal";
}

cwStageLayer::~cwStageLayer()
{
	m_nVecPU.clear();
	CW_SAFE_RELEASE_NULL(m_pCamera);
}

CWVOID cwStageLayer::setPUList(cwVector<cwPUStageLayer*>& vecPU)
{
	m_nVecPU.clear();

	for (auto pPU : vecPU) {
		pPU->setStageLayer(this);
		m_nVecPU.pushBack(pPU);
	}
}

CWVOID cwStageLayer::addPU(cwPUStageLayer* pPUStageLayer)
{
	if (pPUStageLayer) {
		pPUStageLayer->setStageLayer(this);
		m_nVecPU.pushBack(pPUStageLayer);
	}
}

cwPUStageLayer* cwStageLayer::getPU(ePURenderType eType)
{
	for (auto pPU : m_nVecPU) {
		if (pPU->getType() == eType) return pPU;
	}

	return nullptr;
}

CWVOID cwStageLayer::reset()
{
	m_iPipeLineIndex = 0;
	m_nMapPipeline.clear();

	if (!m_nVecPUBatch.empty()) {
		for (auto pPUBatch : m_nVecPUBatch) {
			pPUBatch->reset();
		}
	}
}

CWVOID cwStageLayer::begin(std::vector<cwRenderNode*>* vecEntities)
{
	reset();

	if (vecEntities)
		addEntities(vecEntities);

	for (auto pPU : m_nVecPU) {
		pPU->begin();
	}
}

CWVOID cwStageLayer::begin(cwRenderNode* pNode)
{
	reset();

	if (!m_pCamera)
		cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pCamera);

	if (pNode) {
		addEntities(pNode);
	}

	for (auto pPU : m_nVecPU) {
		pPU->begin();
	}
}

CWVOID cwStageLayer::render()
{
	//m_eOldRenderState = cwRepertory::getInstance().getDevice()->getRenderState();
	//cwRepertory::getInstance().getDevice()->setRenderState(m_eRenderState);

	for (CWUINT i = 0; i < m_iPipeLineIndex; ++i) {
		m_nPipeline[i].render();
	}
}

CWVOID cwStageLayer::end()
{
	for (auto pPU : m_nVecPU) {
		pPU->end();
	}

	//cwRepertory::getInstance().getDevice()->setRenderState(m_eOldRenderState);
}

CWVOID cwStageLayer::addEntities(std::vector<cwRenderNode*>* vecEntities)
{
	for (auto it = vecEntities->begin(); it != vecEntities->end(); ++it) {
		cwRenderPipeline* pPiepline = getPipeline(*it);
		if (pPiepline)
			pPiepline->addEntity(*it, this);
	}
}

CWVOID cwStageLayer::addEntities(cwRenderNode* pNode)
{
	cwRenderPipeline* pPiepline = getPipeline(pNode);
	if (pPiepline)
		pPiepline->addEntity(pNode, this);
}

cwRenderPipeline* cwStageLayer::getPipeline(cwRenderNode* pEntity)
{
	cwRenderPipeline* pPipeLine = nullptr;
	cwEffect* pEffect = pEntity->getEffect();
	cwShader* pShader = nullptr;
	if (pEffect)
		pShader = pEffect->getShader();

	if (m_nMapPipeline.find(pShader) != m_nMapPipeline.end()) {
		pPipeLine = m_nMapPipeline[pShader];
	}
	else {
		pPipeLine = getUnusePipeline(pShader);
	}

	if (pPipeLine && pPipeLine->full()) {
		m_nMapPipeline.erase(pShader);
		pPipeLine = nullptr;

		pPipeLine = getUnusePipeline(pShader);
	}

	return pPipeLine;
}

cwRenderPipeline* cwStageLayer::getUnusePipeline(cwShader* pShader)
{
	if (m_iPipeLineIndex < CW_STAGE_PIPELINE_SIZE) {
		cwRenderPipeline* pPipeLine = &(m_nPipeline[m_iPipeLineIndex++]);
		m_nMapPipeline[pShader] = pPipeLine;

		pPipeLine->reset();
		pPipeLine->setShader(pShader);
		return pPipeLine;
	}

	return nullptr;
}

CWVOID cwStageLayer::setBatchPUList(cwVector<cwPUBatch*>& vecPU)
{
	m_nVecPUBatch.clear();

	for (auto pPU : vecPU) {
		pPU->setStageLayer(this);
		m_nVecPUBatch.pushBack(pPU);
	}
}

CWVOID cwStageLayer::batchPUUpdate(cwRenderBatch* pBatch)
{
	if (!m_nVecPUBatch.empty()) {
		for (auto pPUBatch : m_nVecPUBatch) {
			pPUBatch->updateBatch(pBatch);
		}
	}
}

CWVOID cwStageLayer::setCamera(cwCamera* pCamera)
{
	if (m_pCamera == pCamera) return;
	CW_SAFE_RETAIN(pCamera);
	CW_SAFE_RELEASE_NULL(m_pCamera);
	m_pCamera = pCamera;
}

CWVOID cwStageLayer::setCamera(const CWSTRING& strCamName)
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera(strCamName);
	if (pCamera) {
		this->setCamera(pCamera);
		this->setCameraName(strCamName);
	}
}

NS_MINIR_END
