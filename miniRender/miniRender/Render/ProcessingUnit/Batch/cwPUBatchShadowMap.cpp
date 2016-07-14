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

#include "cwPUBatchShadowMap.h"
#include "Render/Stage/Shadow/cwShadowMapStage.h"
#include "Render/cwRenderBatch.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwPUBatchShadowMap* cwPUBatchShadowMap::create()
{
	cwPUBatchShadowMap* pPUBatch = new cwPUBatchShadowMap();
	if (pPUBatch) {
		pPUBatch->autorelease();
		return pPUBatch;
	}

	return nullptr;
}

cwPUBatchShadowMap::cwPUBatchShadowMap() :
m_pShader(nullptr),
m_pShadowStage(nullptr)
{

}

cwPUBatchShadowMap::~cwPUBatchShadowMap()
{
	m_pShader = nullptr;
	m_pShadowStage = nullptr;
}

CWVOID cwPUBatchShadowMap::reset()
{
	m_pShader = nullptr;
}

CWVOID cwPUBatchShadowMap::bindingStage(cwStage* pStage)
{
	if (!pStage) return;
	m_pShadowStage = dynamic_cast<cwShadowMapStage*>(pStage);
}

CWVOID cwPUBatchShadowMap::updateBatch(cwRenderBatch* pBatch)
{
	if (pBatch && pBatch->m_pEffect) {
		cwShader* pBatchShader = pBatch->m_pEffect->getShader();
		if (pBatchShader != m_pShader)  {
			m_pShader = pBatchShader;
			updateShaderTextureParam();
		}
		
		updateShaderTransParam(pBatch);
	}
}

CWVOID cwPUBatchShadowMap::updateShaderTextureParam()
{
	if (m_pShader && m_pShadowStage) {
		cwTexture* pShadowMap = m_pShadowStage->getRenderTexture();
		if (!m_nShadowMapTextureName.empty() && pShadowMap) {
			if (m_pShader->hasVariable(m_nShadowMapTextureName)) {
				m_pShader->setVariableTexture(m_nShadowMapTextureName, pShadowMap);
			}
		}
	}
}

CWVOID cwPUBatchShadowMap::updateShaderTransParam(cwRenderBatch* pBatch)
{
	if (m_pShader && m_pShadowStage) {
		cwMatrix4X4* pShadowTrans = m_pShadowStage->getShadowTransform();
		if (!m_nShadowMapTransName.empty() && pShadowTrans) {
			if (m_pShader->hasVariable(m_nShadowMapTransName)) {
				m_pShader->setVariableMatrix(m_nShadowMapTransName, pBatch->m_nWorldTrans*(*pShadowTrans));
			}
		}
	}
}

NS_MINIR_END
