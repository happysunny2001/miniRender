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

#include "cwPUStageLayerBlend.h"
#include "Blend/cwBlend.h"
#include "Render/Stage/cwStageLayer.h"
#include "Render/cwRenderPipeline.h"
#include "Render/cwRenderBatch.h"

NS_MINIR_BEGIN

cwPUStageLayerBlend* cwPUStageLayerBlend::create()
{
	cwPUStageLayerBlend* pPUStageLayer = new cwPUStageLayerBlend();
	if (pPUStageLayer) {
		pPUStageLayer->autorelease();
		return pPUStageLayer;
	}

	return nullptr;
}

cwPUStageLayerBlend::cwPUStageLayerBlend():
m_pBlend(nullptr)
{
	m_eType = ePUStageLayerBlend;
}

cwPUStageLayerBlend::~cwPUStageLayerBlend()
{
	CW_SAFE_RELEASE_NULL(m_pBlend);
}

CWVOID cwPUStageLayerBlend::setBlend(cwBlend* pBlend)
{
	CW_SAFE_RETAIN(pBlend);
	CW_SAFE_RELEASE_NULL(m_pBlend);
	m_pBlend = pBlend;
}

CWVOID cwPUStageLayerBlend::begin()
{
	if (!m_pStageLayer) return;

	std::unordered_map<cwShader*, cwRenderPipeline*>& renderPipelines = m_pStageLayer->getRenderPipeline();
	for (auto pipeline : renderPipelines) {
		std::vector<cwRenderBatch>& renderBatches = pipeline.second->getRenderBatch();
		CWUINT iCount = pipeline.second->getBatchCount();
		for (CWUINT i = 0; i < iCount; ++i) {
			renderBatches[i].m_pBlend = this->m_pBlend;
		}
	}
}

CWVOID cwPUStageLayerBlend::end()
{

}

NS_MINIR_END
