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

NS_MINIR_BEGIN

cwStageLayer::cwStageLayer():
m_eType(eStageLayerNormal)
{

}

cwStageLayer::~cwStageLayer()
{

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

CWVOID cwStageLayer::addEntities(std::vector<cwEntity*>& vecEntities, cwEffect* pEffect)
{
	cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
	pPipeline->reset();

	for (auto pEntity : vecEntities) {
		pEntity->transform();

		CWBOOL bRet = pPipeline->addEntity(pEntity);
		if (!bRet) {
			if (m_iPipeLineIndex >= CW_STAGE_PIPELINE_SIZE)
				break;
			else {
				pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
				pPipeline->reset();
			}
		}
	}
}

CWVOID cwStageLayer::addEntities(std::vector<cwEntity*>& vecEntities)
{
	cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
	pPipeline->reset();

	for (auto pEntity : vecEntities) {
		pEntity->transform();

		CWBOOL bRet = pPipeline->addEntity(pEntity, nullptr);
		if (!bRet) {
			if (m_iPipeLineIndex >= CW_STAGE_PIPELINE_SIZE)
				break;
			else {
				pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
				pPipeline->reset();
			}
		}
	}
}

cwRenderPipeline* cwStageLayer::getPipeline(cwEntity* pEntity)
{
	cwShader* pShader = pEntity->getEffect()->getShader();
	if (m_nMapPipeline.find(pShader) != m_nMapPipeline.end()) {
		return m_nMapPipeline[pShader];
	}
	else {
		if (m_iPipeLineIndex < CW_STAGE_PIPELINE_SIZE) {
			cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
			m_nMapPipeline[pShader] = pPipeline;
			return pPipeline;
		}
	}

	return nullptr;
}

CWVOID cwStageLayer::render()
{
	for (CWUINT i = 0; i < m_iPipeLineIndex; ++i) {
		m_nPipeline[i].render();
	}
}

CWVOID cwStageLayer::end()
{

}

NS_MINIR_END
