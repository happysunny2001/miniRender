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

NS_MINIR_BEGIN

cwStageLayer::cwStageLayer():
m_eFilterType(eStageLayerFliterEntity),
m_bTransparent(CWFALSE),
m_pStageBlend(nullptr),
m_pStageStencil(nullptr)
{

}

cwStageLayer::~cwStageLayer()
{
	CW_SAFE_RELEASE_NULL(m_pStageBlend);
	CW_SAFE_RELEASE_NULL(m_pStageStencil);
}

CWVOID cwStageLayer::setBlend(cwBlend* pBlend)
{
	CW_SAFE_RETAIN(pBlend);
	CW_SAFE_RELEASE_NULL(m_pStageBlend);
	m_pStageBlend = pBlend;
}

CWVOID cwStageLayer::setStencil(cwStencil* pStencil)
{
	CW_SAFE_RETAIN(pStencil);
	CW_SAFE_RELEASE_NULL(m_pStageStencil);
	m_pStageStencil = pStencil;
}

CWVOID cwStageLayer::reset()
{
	m_iPipeLineIndex = 0;
	m_nMapPipeline.clear();
}

CWVOID cwStageLayer::begin(std::vector<cwEntity*>& vecEntities, cwEffect* pEffect)
{
	reset();

	if (pEffect == nullptr) {
		addEntities(vecEntities);
	}
	else {
		addEntities(vecEntities, pEffect);
	}
}

CWVOID cwStageLayer::render()
{
	cwRepertory::getInstance().getDevice()->setBlend(m_pStageBlend);
	cwRepertory::getInstance().getDevice()->setStencil(m_pStageStencil);

	for (CWUINT i = 0; i < m_iPipeLineIndex; ++i) {
		m_nPipeline[i].render();
	}
}

CWVOID cwStageLayer::end()
{

}

CWVOID cwStageLayer::addEntities(std::vector<cwEntity*>& vecEntities, cwEffect* pEffect)
{
	cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
	pPipeline->reset();
	pPipeline->setShader(pEffect->getShader());

	for (auto pEntity : vecEntities) {
		pEntity->transform();

		CWBOOL bRet = pPipeline->addEntity(pEntity, pEffect);
		if (!bRet) {
			if (m_iPipeLineIndex < CW_STAGE_PIPELINE_SIZE) {
				pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
				pPipeline->reset();
				pPipeline->setShader(pEffect->getShader());
			}
			else
				break;
		}
	}
}

CWVOID cwStageLayer::addEntities(std::vector<cwEntity*>& vecEntities)
{
	for (auto pEntity : vecEntities) {
		pEntity->transform();

		cwRenderPipeline* pPiepline = getPipeline(pEntity);
		if (pPiepline)
			pPiepline->addEntity(pEntity);
	}
}

cwRenderPipeline* cwStageLayer::getPipeline(cwEntity* pEntity)
{
	cwRenderPipeline* pPipeLine = nullptr;
	cwShader* pShader = pEntity->getEffect()->getShader();
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
