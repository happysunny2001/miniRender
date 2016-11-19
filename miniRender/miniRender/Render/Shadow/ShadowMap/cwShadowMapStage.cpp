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
#include "cwShadowMapConstant.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Texture/cwTexture.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffect.h"
#include "cwShadowMapStageLayer.h"
#include "Sprite/cwSprite.h"
#include "Engine/cwEngine.h"
#include "Entity/cwScene.h"

NS_MINIR_BEGIN

cwShadowMapStage::cwShadowMapStage() :
m_pShadowStageLayer(nullptr),
m_pShadowSprite(nullptr)
{

}

cwShadowMapStage::~cwShadowMapStage()
{
	CW_SAFE_RELEASE_NULL(m_pShadowStageLayer);
	CW_SAFE_RELEASE_NULL(m_pShadowSprite);
}

CWBOOL cwShadowMapStage::init()
{
	if (!cwStage::init()) return CWFALSE;
	if (!buildShadowMapStageLayer()) return CWFALSE;

	return CWTRUE;
}

CWVOID cwShadowMapStage::bindingResultParameter(cwShader* pShader)
{
	if (pShader && m_pShadowStageLayer) {
		pShader->setVariableTexture(CW_SHADOW_MAP_PARAM_TEXTURE, m_pResultRenderTexture);
		pShader->setVariableMatrix(CW_SHADOW_MAP_PARAM_MATRIX, m_pShadowStageLayer->getShadowTransform());
	}
}

CWBOOL cwShadowMapStage::buildRenderTexture()
{
	m_pResultRenderTexture = cwRepertory::getInstance().getDevice()->createDSTexture(CW_SHADOW_MAP_WIDTH, CW_SHADOW_MAP_HEIGHT, CWTRUE);
	if (!m_pResultRenderTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pResultRenderTexture);

	return CWTRUE;
}

CWBOOL cwShadowMapStage::buildShadowMapStageLayer()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	cwShader* pShadowShader = repertory.getShaderManager()->createShader(CW_SHADOW_MAP_SHADER_FILE);
	if (!pShadowShader) return CWFALSE;

	cwEffect* pShadowEffect = cwEffect::create();
	if (!pShadowEffect) return CWFALSE;
	pShadowEffect->setShader(pShadowShader);
	pShadowEffect->setTech(CW_SHADOW_MAP_SHADER_TECH);

	m_pShadowStageLayer = cwShadowMapStageLayer::create();
	if (!m_pShadowStageLayer) return nullptr;
	CW_SAFE_RETAIN(m_pShadowStageLayer);

	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;
	
	m_pShadowStageLayer->setCamera(pCamera);
	m_pShadowStageLayer->setName(CW_SHADOW_MAP_STAGE_LAYER_NAME);
	m_pShadowStageLayer->setRenderTarget(nullptr);
	m_pShadowStageLayer->setDepthStencil(m_pResultRenderTexture);
	m_pShadowStageLayer->setIsRefreshRenderTarget(CWTRUE);
	m_pShadowStageLayer->setIsClearColor(CWFALSE);
	m_pShadowStageLayer->setIsClearDepth(CWTRUE);
	m_pShadowStageLayer->setIsClearStencil(CWFALSE);
	m_pShadowStageLayer->setFilterType(eStageLayerFliterScene);
	m_pShadowStageLayer->setUniformEffect(pShadowEffect);

	this->addStageLayer(m_pShadowStageLayer);

	return CWTRUE;
}

CWVOID cwShadowMapStage::showResult(const cwVector2D& pos, const cwVector2D& scale)
{
	if (!m_pShadowSprite) {
		m_pShadowSprite = cwSprite::create();

		m_pShadowSprite->setTexture(m_pResultRenderTexture);
		m_pShadowSprite->setScale(scale.x, scale.y, 1.0f);
		m_pShadowSprite->setPosition(pos.x, pos.y);
		m_pShadowSprite->setAnchorPoint(cwVector2D(-0.5f, -0.5f));

		cwShader* pShader = cwRepertory::getInstance().getShaderManager()->createShader("SpriteRenderTechnique.hlsl");
		cwEffect* pSpriteEffect = cwEffect::create();
		pSpriteEffect->setShader(pShader);
		pSpriteEffect->setTech("SpriteGrayTech");
		m_pShadowSprite->setEffect(pSpriteEffect);
		CW_SAFE_RETAIN(m_pShadowSprite);
	}

	if (!m_pShadowSprite->getParent())
		cwRepertory::getInstance().getEngine()->getCurrScene()->addChild2D(m_pShadowSprite);
}

CWVOID cwShadowMapStage::render()
{
	cwStage::render();

	CWUINT width = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT height = cwRepertory::getInstance().getUInt(gValueWinHeight);
	this->showResult(cwVector2D(-(CWFLOAT)(width / 2), -(CWFLOAT)(height / 2)), cwVector2D(0.1f, 0.1f));
}

NS_MINIR_END
