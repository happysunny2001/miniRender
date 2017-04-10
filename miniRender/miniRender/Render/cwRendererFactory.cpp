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

#include "cwRendererFactory.h"
#include "cwRenderer.h"
#include "TBDR/cwTBDRStage.h"
#include "Shadow/ShadowMap/cwShadowMapStage.h"
#include "Stage/cwStage.h"
#include "Stage/cwStageLayer.h"
#include "AO/SSAO/cwSSAOConstants.h"

NS_MINIR_BEGIN

cwRendererFactory* cwRendererFactory::create()
{
	cwRendererFactory* pRendererFactory = new cwRendererFactory();
	if (pRendererFactory) {
		pRendererFactory->autorelease();
	}

	return pRendererFactory;
}

cwRendererFactory::cwRendererFactory()
{

}

cwRendererFactory::~cwRendererFactory()
{

}

cwRenderer* cwRendererFactory::createTBDRRenderer()
{
	cwRenderer* pTBDRRenderer = cwRenderer::create();
	if (!pTBDRRenderer) return nullptr;

	cwShadowMapStage* pShadowMapStage = cwShadowMapStage::create();
	pTBDRRenderer->addStage(pShadowMapStage);

	cwStage* pTBDRStage = cwTBDRStage::create();
	pTBDRRenderer->addStage(pTBDRStage);

	cwPipelineNode* pTBDRCoreLayer = pTBDRStage->getChildPipelineNode(CW_TBDR_CORE_LAYER_NAME);
	if (pTBDRCoreLayer) {
		pTBDRCoreLayer->addOuterPipeline(pShadowMapStage);
	}

	cwStage* pSSAOStage = dynamic_cast<cwStage*>(pTBDRStage->getChildPipelineNode(CW_SSAO_STAGE_NAME));

	CWFLOAT width = (CWFLOAT)cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWFLOAT height = (CWFLOAT)cwRepertory::getInstance().getUInt(gValueWinHeight);

	pShadowMapStage->showResult(cwVector2D(-width / 2, -height / 2), cwVector2D(0.1f, 0.1f));

	if (pSSAOStage)
		pSSAOStage->showResult(cwVector2D(0, -height / 2), cwVector2D(0.25f, 0.25f));

	return pTBDRRenderer;
}

NS_MINIR_END
