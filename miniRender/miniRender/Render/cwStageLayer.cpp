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

#include "cwStageLayer.h"
#include "Entity/cwEntity.h"
#include "Effect/cwEffect.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "ProcessingUnit/StageLayer/cwPUStageLayer.h"

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
m_eRenderState(eRenderStateSolid)
{
	reset();
}

cwStageLayer::~cwStageLayer()
{
	m_nVecPU.clear();
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
}

CWVOID cwStageLayer::begin(cwVector<cwRenderNode*>* vecEntities)
{
	reset();

	if (vecEntities)
		addEntities(*vecEntities);

	for (auto pPU : m_nVecPU) {
		pPU->begin();
	}
}

CWVOID cwStageLayer::render()
{
	m_eOldRenderState = cwRepertory::getInstance().getDevice()->getRenderState();
	cwRepertory::getInstance().getDevice()->setRenderState(m_eRenderState);

	for (CWUINT i = 0; i < m_iPipeLineIndex; ++i) {
		m_nPipeline[i].render();
	}
}

CWVOID cwStageLayer::end()
{
	for (auto pPU : m_nVecPU) {
		pPU->end();
	}

	cwRepertory::getInstance().getDevice()->setRenderState(m_eOldRenderState);
}

CWVOID cwStageLayer::addEntities(cwVector<cwRenderNode*>& vecEntities)
{
	for (auto pEntity : vecEntities) {
		cwRenderPipeline* pPiepline = getPipeline(pEntity);
		if (pPiepline)
			pPiepline->addEntity(pEntity, this);
	}
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

NS_MINIR_END
