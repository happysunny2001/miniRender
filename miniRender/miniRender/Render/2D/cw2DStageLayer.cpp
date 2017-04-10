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

#include "cw2DStageLayer.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Entity/cwScene.h"
#include "Engine/cwEngine.h"
#include "Sprite/cwRenderNode2D.h"
#include "effect/cwEffect.h"
#include "Sprite/cwFrameRateLabel.h"
#include "Blend/cwBlend.h"

NS_MINIR_BEGIN

cw2DStageLayer::cw2DStageLayer()
{

}

cw2DStageLayer::~cw2DStageLayer()
{

}

CWVOID cw2DStageLayer::batchScene2DNodes()
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();

	if (pCurrScene) {
		std::vector<cwRenderNode2D*>& vecNodes2D = pCurrScene->getRenderNodes2D();
		for (auto it = vecNodes2D.begin(); it != vecNodes2D.end(); ++it) {
			this->addRenderNode((*it));
		}
	}

	renderFrameLabel();
}

CWVOID cw2DStageLayer::renderFrameLabel()
{
	if (cwRepertory::getInstance().getEngine()->getFrameEnable()) {
		cwFrameRateLabel* pLabel = cwRepertory::getInstance().getEngine()->getFrameRateLabel();
		this->addRenderNode(pLabel);
	}
}

CWBOOL cw2DStageLayer::buildBlend()
{
	cwBlendData blendData;

	blendData.bAlphaToCoverage = CWFALSE;
	blendData.bIndependentBlend = CWFALSE;
	blendData.bEnable = CWTRUE;
	blendData.uColorEnable = eColorWriteEnableAll;

	blendData.srcBlend = eBlendFactorOne;
	blendData.dstBlend = eBlendFactorInvSrcAlpha;
	blendData.blendOp = eBlendOpAdd;
	blendData.srcBlendAlpha = eBlendFactorOne;
	blendData.dstBlendAlpha = eBlendFactorZero;
	blendData.blendOpAlpha = eBlendOpAdd;

	m_pBlend = cwRepertory::getInstance().getDevice()->createBlend(blendData);
	CW_SAFE_RETAIN(m_pBlend);
	if (m_pBlend) return CWTRUE;

	return CWFALSE;
}

NS_MINIR_END
