﻿/*
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

#include "cwRenderPipeline.h"
#include "Entity/cwEntity.h"

NS_MINIR_BEGIN

cwRenderPipeline::cwRenderPipeline():
m_iBatchIndex(0)
{
	m_nVecBatch.resize(CW_PIPELINE_BATCH_SIZE);
}

cwRenderPipeline::~cwRenderPipeline()
{

}

CWVOID cwRenderPipeline::reset()
{
	m_iBatchIndex = 0;
}

cwRenderBatch* cwRenderPipeline::getNextAvailableBatch()
{
	if (m_iBatchIndex < m_nVecBatch.size()) {
		return &(m_nVecBatch[m_iBatchIndex++]);
	}

	return nullptr;
}

CWBOOL cwRenderPipeline::addEntity(cwEntity* pEntity)
{
	cwRenderBatch* pBatch = getNextAvailableBatch();
	if (!pBatch) return CWFALSE;

	pBatch->m_pEntity = pEntity;
	pBatch->m_pEffect = pEntity->getEffect();
	return CWTRUE;
}

CWBOOL cwRenderPipeline::addEntity(cwEntity* pEntity, cwEffect* pEffect)
{
	cwRenderBatch* pBatch = getNextAvailableBatch();
	if (!pBatch) return CWFALSE;

	pBatch->m_pEntity = pEntity;
	pBatch->m_pEffect = pEffect;
	return CWTRUE;
}

CWVOID cwRenderPipeline::render()
{
	for (CWUINT i = 0; i < m_iBatchIndex; ++i) {
		m_nVecBatch[i].render();
	}
}

NS_MINIR_END
