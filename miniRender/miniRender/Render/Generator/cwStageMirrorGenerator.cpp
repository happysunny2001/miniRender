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

#include "cwStageMirrorGenerator.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Device/cwDevice.h"
#include "Stencil/cwStencil.h"
#include "Blend/cwBlend.h"
#include "Entity/cwScene.h"
#include "Entity/cwEntity.h"
#include "Entity/cwMirror.h"
#include "Render/cwStage.h"
#include "Render/cwStageLayer.h"
#include "Render/cwRenderer.h"
#include "Render/ProcessingUnit/StageLayer/cwPUStageLayer.h"
#include "Render/ProcessingUnit/StageLayer/cwPUStageLayerStencil.h"
#include "Render/ProcessingUnit/StageLayer/cwPUStageLayerBlend.h"
#include "Render/ProcessingUnit/StageLayer/cwPUStageLayerWorldTrans.h"
#include "Render/ProcessingUnit/StageLayer/cwPUStageLayerLightTrans.h"

NS_MINIR_BEGIN

cwStageMirrorGenerator* cwStageMirrorGenerator::create()
{
	cwStageMirrorGenerator* pGenerator = new cwStageMirrorGenerator();
	if (pGenerator) {
		pGenerator->autorelease();
		return pGenerator;
	}

	return nullptr;
}

cwStageMirrorGenerator::cwStageMirrorGenerator() : 
m_pStage(nullptr),
m_pMirrorStageLayer(nullptr),
m_pStencilDrawRef(nullptr),
m_pBlendTransparent(nullptr)
{
	buildBlend();
	buildStencil();
	buildStage();
}

cwStageMirrorGenerator::~cwStageMirrorGenerator()
{
	CW_SAFE_RELEASE_NULL(m_pStage);
	CW_SAFE_RELEASE_NULL(m_pMirrorStageLayer);
	CW_SAFE_RELEASE_NULL(m_pStencilDrawRef);
	CW_SAFE_RELEASE_NULL(m_pBlendTransparent);

	m_nVecEntityStageLayer.clear();
}

CWVOID cwStageMirrorGenerator::generate()
{
	cwEngine* pEngine = cwRepertory::getInstance().getEngine();
	cwCamera* pStageCamera = pEngine->getRenderer()->getCurrRenderStage()->getCamera();
	m_pStage->setCamera(pStageCamera);
	m_pStage->clearStageLayer(CWFALSE);

	cwVector<cwRenderNode*>* pVecNodes = pEngine->getVisibleNodes(pStageCamera, eSceneObjectMirror);

	CWUINT iIndex = 0;
	for (auto it = pVecNodes->begin(); it != pVecNodes->end(); ++it) {
		cwStageLayer* pStageLayer = m_nVecEntityStageLayer.at(iIndex);
		cwMirror* pMirror = static_cast<cwMirror*>(*it);

		cwPUStageLayerWorldTrans* pPUStageLayerWorldTrans = static_cast<cwPUStageLayerWorldTrans*>(pStageLayer->getPU(ePUStageLayerWorldTrans));
		if (pPUStageLayerWorldTrans) {
			pPUStageLayerWorldTrans->setWorldTrans(pMirror->getReclectMatrix());
		}

		cwPUStageLayerLightTrans* pPUStageLayerLightTrans = static_cast<cwPUStageLayerLightTrans*>(pStageLayer->getPU(ePUStageLayerLightTrans));
		if (pPUStageLayerLightTrans) {
			pPUStageLayerLightTrans->setLightTrans(pMirror->getReclectMatrix());
		}

		m_pStage->addStageLayer(pStageLayer);
		if (++iIndex >= pVecNodes->size()) break;
	}

	m_pStage->addStageLayer(m_pMirrorStageLayer);
	pEngine->getRenderer()->addStageRealTime(m_pStage);
}

CWVOID cwStageMirrorGenerator::buildStage()
{
	m_pStage = cwStage::create();
	m_pMirrorStageLayer = cwStageLayer::create();

	if (!m_pStage || !m_pMirrorStageLayer) return;

	CW_SAFE_RETAIN(m_pStage);
	CW_SAFE_RETAIN(m_pMirrorStageLayer);

	m_pStage->setRefreshRenderTarget(CWFALSE);
	m_pStage->setName("MirrorStage");

	for (CWUINT i = 0; i < CW_MIRROR_CNT; ++i) {
		cwStageLayer* pStageLayerEntity = cwStageLayer::create();
		if (pStageLayerEntity) {
			pStageLayerEntity->setFliterType(eStageLayerFliterEntity);

			cwPUStageLayerStencil* pPUStageLayerStencil = cwPUStageLayerStencil::create();
			if (pPUStageLayerStencil) {
				pPUStageLayerStencil->setStencil(m_pStencilDrawRef);
				pStageLayerEntity->addPU(pPUStageLayerStencil);
			}

			cwPUStageLayerWorldTrans* pPUStageLayerWorldTrans = cwPUStageLayerWorldTrans::create();
			if (pPUStageLayerWorldTrans) {
				pStageLayerEntity->addPU(pPUStageLayerWorldTrans);
			}

			cwPUStageLayerLightTrans* pPUStageLayerLightTrans = cwPUStageLayerLightTrans::create();
			if (pPUStageLayerLightTrans) {
				pStageLayerEntity->addPU(pPUStageLayerLightTrans);
			}

			pStageLayerEntity->setRenderState(eRenderStateCW);

			m_nVecEntityStageLayer.pushBack(pStageLayerEntity);
		}
	}

	cwPUStageLayerBlend* pPUStageLayerBlend = cwPUStageLayerBlend::create();
	if (pPUStageLayerBlend) {
		pPUStageLayerBlend->setBlend(m_pBlendTransparent);
		m_pMirrorStageLayer->addPU(pPUStageLayerBlend);
	}
	m_pMirrorStageLayer->setFliterType(eStageLayerFliterMirror);
}

CWVOID cwStageMirrorGenerator::buildStencil()
{
	cwStencilData stencilDrawRef;
	stencilDrawRef.depthEnable = CWTRUE;
	stencilDrawRef.depthWriteMask = eDepthWriteMaskAll;
	stencilDrawRef.depthFunc = eComparisonLess;
	stencilDrawRef.stencilEnable = CWTRUE;
	stencilDrawRef.stencilReadMask = 0xff;
	stencilDrawRef.stencilWriteMask = 0xff;

	stencilDrawRef.frontStencilFailOp = eStencilOpKeep;
	stencilDrawRef.frontStencilDepthFailOp = eStencilOpKeep;
	stencilDrawRef.frontStencilPassOp = eStencilOpKeep;
	stencilDrawRef.frontStencilFunc = eComparisonEqual;

	stencilDrawRef.backStencilFailOp = eStencilOpKeep;
	stencilDrawRef.backStencilDepthFailOp = eStencilOpKeep;
	stencilDrawRef.backStencilPassOp = eStencilOpKeep;
	stencilDrawRef.backStencilFunc = eComparisonEqual;
	m_pStencilDrawRef = cwRepertory::getInstance().getDevice()->createStencil(stencilDrawRef);
	m_pStencilDrawRef->setStencilRef(1);
	CW_SAFE_RETAIN(m_pStencilDrawRef);
}

CWVOID cwStageMirrorGenerator::buildBlend()
{
	cwBlendData blendTrans;
	blendTrans.bEnable = CWTRUE;
	blendTrans.srcBlend = eBlendFactorSrcAlpha;
	blendTrans.dstBlend = eBlendFactorInvSrcAlpha;
	blendTrans.blendOp = eBlendOpAdd;
	blendTrans.srcBlendAlpha = eBlendFactorOne;
	blendTrans.dstBlendAlpha = eBlendFactorZero;
	blendTrans.blendOpAlpha = eBlendOpAdd;
	blendTrans.uColorEnable = eColorWriteEnableAll;

	m_pBlendTransparent = cwRepertory::getInstance().getDevice()->createBlend(blendTrans);
	CW_SAFE_RETAIN(m_pBlendTransparent);
}

NS_MINIR_END
