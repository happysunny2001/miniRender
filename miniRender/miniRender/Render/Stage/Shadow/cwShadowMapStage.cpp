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

#include "cwShadowMapStage.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Render/Stage/Layer/cwShadowMapStageLayer.h"
#include "Sprite/cwSprite.h"
#include "Engine/cwEngine.h"
#include "Base/cwUtils.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffect.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "ViewPort/cwViewPort.h"

NS_MINIR_BEGIN

cwShadowMapStage* cwShadowMapStage::create()
{
	cwShadowMapStage* pStage = new cwShadowMapStage();
	if (pStage) {
		pStage->autorelease();
		return pStage;
	}

	return nullptr;
}

cwShadowMapStage::cwShadowMapStage() :
m_pViewPort(nullptr),
m_pShadowMapLayer(nullptr),
m_pShadowSprite(nullptr)
{
	m_eType = eStageTypeShadowMap;
}

cwShadowMapStage::~cwShadowMapStage()
{
	CW_SAFE_RELEASE_NULL(m_pViewPort);
	CW_SAFE_RELEASE_NULL(m_pShadowSprite);
	m_pShadowMapLayer = nullptr;
}

CWVOID cwShadowMapStage::showShadowMapSprite()
{
	if (!m_pShadowSprite) {
		m_pShadowSprite = cwSprite::create();
		m_pShadowSprite->setTexture(this->getRenderTexture());

		CWUINT uScreenWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
		CWUINT uScreenHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);
		CWFLOAT fSpriteWidth = uScreenWidth / 5.0f;
		CWFLOAT fScale = fSpriteWidth / m_pRenderTarget->getWidth();
		CWFLOAT fSpriteHeight = m_pRenderTarget->getHeight()*fScale;

		m_pShadowSprite->setScale(fScale, fScale, 1.0f);
		m_pShadowSprite->setPosition((uScreenWidth - fSpriteWidth)*0.5f, -(uScreenHeight - fSpriteHeight)*0.5f);

		cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderTex);
		cwEffect* pSpriteEffect = cwEffect::create();
		pSpriteEffect->setShader(pShader);
		pSpriteEffect->setTech("GrayTech");
		m_pShadowSprite->setEffect(pSpriteEffect);
		CW_SAFE_RETAIN(m_pShadowSprite);
	}

	if (!m_pShadowSprite->getParent())
		cwRepertory::getInstance().getEngine()->addNode2D(m_pShadowSprite);
}

CWVOID cwShadowMapStage::hideShadowMapSprite()
{
	if (m_pShadowSprite) {
		m_pShadowSprite->removeFromParent();
	}
}

CWVOID cwShadowMapStage::reset()
{

}

CWVOID cwShadowMapStage::begin()
{
	reset();

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();

	pDevice->pushDeviceStatus();

	pDevice->setRenderTarget(m_pRenderTarget);
	pDevice->setViewPort(m_pViewPort);
	pDevice->beginDraw(m_bClearColor, m_bClearDepth, m_bClearStencil);
}

CWVOID cwShadowMapStage::render()
{
	cwStage::render();
}

CWVOID cwShadowMapStage::end()
{
	cwStage::end();
	cwRepertory::getInstance().getDevice()->popDeviceStatus();
}

CWVOID cwShadowMapStage::setRenderTexture(cwTexture* pRenderTexture)
{
	cwStage::setRenderTexture(pRenderTexture);

	CW_SAFE_RELEASE_NULL(m_pViewPort);
	m_pViewPort = cwRepertory::getInstance().getDevice()->createViewPort(
		0, 0, m_pRenderTarget->getWidth(), m_pRenderTarget->getHeight());
	CW_SAFE_RETAIN(m_pViewPort);

	showShadowMapSprite();
}

cwMatrix4X4* cwShadowMapStage::getShadowTransform()
{
	if (m_pShadowMapLayer) {
		return m_pShadowMapLayer->getShadowTransform();
	}

	if (!m_nVecLayer.empty()) {
		for (auto pLayer : m_nVecLayer) {
			if (pLayer->getLayerType() == "ShadowMap") {
				m_pShadowMapLayer = dynamic_cast<cwShadowMapStageLayer*>(pLayer);
				break;
			}
		}

		return m_pShadowMapLayer->getShadowTransform();
	}

	return nullptr;
}

NS_MINIR_END
