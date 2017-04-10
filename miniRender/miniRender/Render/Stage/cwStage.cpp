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
#include "Sprite/cwSprite.h"

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
m_pResultRenderTexture(nullptr),
m_bShowResult(CWFALSE)
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

CWVOID cwStage::showResult(const cwVector2D& pos, const cwVector2D& scale)
{
	m_bShowResult = CWTRUE;
	m_nResultPos = pos;
	m_nResultScale = scale;
}

CWVOID cwStage::showResult()
{

}

cwTexture* cwStage::getRenderResult() const
{
	return m_pResultRenderTexture;
}

cwPipelineNode* cwStage::getChildPipelineNode(const CWSTRING& name)
{
	for (auto it = m_nVecStageLayers.begin(); it != m_nVecStageLayers.end(); ++it) {
		if ((*it)->name() == name) return (*it);
	}

	return nullptr;
}

CWVOID cwStage::addChildPipelineNode(cwPipelineNode* pPipelineNode)
{
	m_nVecStageLayers.pushBack(pPipelineNode);
}

CWVOID cwStage::removeChildPipelineNode(cwPipelineNode* pPipelineNode)
{
	m_nVecStageLayers.erase(pPipelineNode);
}

CWVOID cwStage::doRender()
{
	this->begin();
	this->render();
	this->end();
}

CWVOID cwStage::begin()
{

}

CWVOID cwStage::render()
{
	for (auto it = m_nVecStageLayers.begin(); it != m_nVecStageLayers.end(); ++it) {
		(*it)->doRender();
	}

	if (m_bShowResult)
		showResult();
}

CWVOID cwStage::end()
{

}

cwSprite* cwStage::createResultSprite(cwTexture* pTexture, cwEffect* pEffect)
{
	cwSprite* pSprite = cwSprite::create();
	if (pSprite) {
		pSprite->setTexture(pTexture);
		pSprite->setScale(m_nResultScale.x, m_nResultScale.y, 1.0f);
		pSprite->setPosition(m_nResultPos.x, m_nResultPos.y);
		pSprite->setAnchorPoint(cwVector2D(-0.5f, -0.5f));
		pSprite->setEffect(pEffect);
	}

	return pSprite;
}

NS_MINIR_END

