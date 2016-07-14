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

#include "cwPUStageLayerEffect.h"
#include "Effect/cwEffect.h"
#include "Render/Stage/Layer/cwStageLayer.h"
#include "Render/cwRenderPipeline.h"
#include "Render/cwRenderBatch.h"

NS_MINIR_BEGIN

cwPUStageLayerEffect* cwPUStageLayerEffect::create()
{
	cwPUStageLayerEffect* pPUStageLayer = new cwPUStageLayerEffect();
	if (pPUStageLayer) {
		pPUStageLayer->autorelease();
		return pPUStageLayer;
	}

	return nullptr;
}

cwPUStageLayerEffect::cwPUStageLayerEffect():
m_pEffect(nullptr)
{
	m_eType = ePUStageLayerEffect;
}

cwPUStageLayerEffect::~cwPUStageLayerEffect()
{
	CW_SAFE_RELEASE_NULL(m_pEffect);
}

CWVOID cwPUStageLayerEffect::setEffect(cwEffect* pEffect)
{
	if (m_pEffect == pEffect) return;
	CW_SAFE_RETAIN(pEffect);
	CW_SAFE_RELEASE_NULL(m_pEffect);
	m_pEffect = pEffect;
}

CWVOID cwPUStageLayerEffect::begin()
{
	if (!m_pEffect) return;

	std::unordered_map<cwShader*, cwRenderPipeline*>& renderPipelines = m_pStageLayer->getRenderPipeline();
	for (auto pipeline : renderPipelines) {
		std::vector<cwRenderBatch>& renderBatches = pipeline.second->getRenderBatch();
		CWUINT iCount = pipeline.second->getBatchCount();
		for (CWUINT i = 0; i < iCount; ++i) {
			renderBatches[i].m_pEffect = m_pEffect;
		}
	}
}

CWVOID cwPUStageLayerEffect::end()
{

}

NS_MINIR_END
