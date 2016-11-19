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

#include "cwStage.h"
#include "cwStageLayer.h"
#include "Texture/cwTexture.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwStage* cwStage::create()
{
	cwStage* pStage = new cwStage();
	if (pStage && pStage->init()) {
		pStage->autorelease();
		return pStage;
	}

	CW_SAFE_DELETE(pStage);
	return nullptr;
}

cwStage* cwStage::create(cwTexture* pRenderTexture)
{
	cwStage* pStage = new cwStage();
	if (pStage && pStage->init(pRenderTexture)) {
		pStage->autorelease();
		return pStage;
	}

	CW_SAFE_DELETE(pStage);
	return nullptr;
}

cwStage::cwStage() :
m_pCurrUsingCamera(nullptr),
m_pResultRenderTexture(nullptr)
{

}

cwStage::~cwStage()
{
	CW_SAFE_RELEASE_NULL(m_pResultRenderTexture);
}

CWBOOL cwStage::init()
{
	if (!buildRenderTexture()) return CWFALSE;
	return CWTRUE;
}

CWBOOL cwStage::init(cwTexture* pRenderTexture)
{
	CW_SAFE_RETAIN(pRenderTexture);
	m_pResultRenderTexture = pRenderTexture;
	return CWTRUE;
}

CWBOOL cwStage::buildRenderTexture()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	CWFLOAT winWidth = static_cast<CWFLOAT>(repertory.getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(repertory.getUInt(gValueWinHeight));

	m_pResultRenderTexture = repertory.getDevice()->createRTTexture(winWidth, winHeight, eFormatR8g8b8a8Unorm, CWTRUE);
	if (!m_pResultRenderTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pResultRenderTexture);

	return CWTRUE;
}

CWVOID cwStage::bindingResultParameter(cwShader* pShader)
{

}

CWVOID cwStage::showResult(const cwVector2D& pos, const cwVector2D& scale)
{

}

cwTexture* cwStage::getRenderResult() const
{
	return m_pResultRenderTexture;
}

cwStageLayer* cwStage::getStageLayer(const CWSTRING& name)
{
	for (auto it = m_nVecStageLayers.begin(); it != m_nVecStageLayers.end(); ++it) {
		if ((*it)->name() == name) return (*it);
	}

	return nullptr;
}

CWVOID cwStage::addStageLayer(cwStageLayer* pStageLayer)
{
	m_nVecStageLayers.pushBack(pStageLayer);
}

CWVOID cwStage::removeStageLayer(cwStageLayer* pStageLayer)
{
	m_nVecStageLayers.erase(pStageLayer);
}

CWVOID cwStage::begin()
{
	m_pCurrUsingCamera = nullptr;
}

CWVOID cwStage::render()
{
	for (auto it = m_nVecStageLayers.begin(); it != m_nVecStageLayers.end(); ++it) {
		cwStageLayer* pStageLayer = (*it);
		m_pCurrUsingCamera = pStageLayer->getCamera();
		pStageLayer->begin();
		pStageLayer->render();
		pStageLayer->end();
	}
}

CWVOID cwStage::end()
{

}

NS_MINIR_END

