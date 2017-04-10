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

#include "cwSSAOStage.h"
#include "Texture/cwTexture.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Device/cwDevice.h"
#include "Shader/cwShaderManager.h"
#include "effect/cwEffect.h"
#include "effect/Parameter/cwEffectTextureParameter.h"
#include "Texture/cwTexture.h"
#include "Entity/cwEntity.h"
#include "Generator/cwGeometryGenerator.h"
#include "Generator/cwRenderObjectGenerator.hpp"
#include "Sprite/cwSprite.h"
#include "Entity/cwScene.h"
#include "cwSSAOStageLayer.h"
#include "cwSSAOConstants.h"

NS_MINIR_BEGIN

cwSSAOStage::cwSSAOStage() :
m_pBlurTexture(nullptr),
m_pScreenQuad(nullptr),
m_pSSAOSprite(nullptr)
{

}

cwSSAOStage::~cwSSAOStage()
{
	CW_SAFE_RELEASE_NULL(m_pBlurTexture);
	CW_SAFE_RELEASE_NULL(m_pScreenQuad);
	CW_SAFE_RELEASE_NULL(m_pSSAOSprite);
}

CWBOOL cwSSAOStage::init()
{
	if (!cwStage::init()) return CWFALSE;
	if (!buildEntity()) return CWFALSE;
	if (!buildSSAOLayer()) return CWFALSE;
	if (!buildBlurTexture()) return CWFALSE;

	if (!buildHorizBlurLayer()) return CWFALSE;
	if (!buildVertBlurLayer()) return CWFALSE;
	if (!buildHorizBlurLayer()) return CWFALSE;
	if (!buildVertBlurLayer()) return CWFALSE;

	this->setName(CW_SSAO_STAGE_NAME);

	return CWTRUE;
}

CWBOOL cwSSAOStage::buildEntity()
{
	cwRenderObjectGenerator<cwVertexPosTex> generator;
	cwRenderObject* pRenderObj = generator.generateQuad("PosTex");

	m_pScreenQuad = cwEntity::create();
	if (!m_pScreenQuad) return CWFALSE;
	m_pScreenQuad->setRenderObject(pRenderObj);
	CW_SAFE_RETAIN(m_pScreenQuad);

	return CWTRUE;
}

CWBOOL cwSSAOStage::buildRenderTexture()
{
	if (cwStage::buildRenderTexture()) {
		if (m_pResultRenderTexture) {
			m_pResultRenderTexture->setName("SSAO");
		}
		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwSSAOStage::buildSSAOLayer()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwShader* pSSAOShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_SSAO_SHADER_FILE);
	if (!pSSAOShader) return CWFALSE;

	cwEffect* pSSAOEffect = cwEffect::create();
	if (!pSSAOEffect) return CWFALSE;
	pSSAOEffect->setShader(pSSAOShader);
	pSSAOEffect->setTech(CW_SSAO_SHADER_TECH);

	cwSSAOStageLayer* pSSAOStageLayer = cwSSAOStageLayer::create();
	if (!pSSAOStageLayer) return CWFALSE;

	pSSAOStageLayer->setName(CW_SSAO_LAYER_NAME);
	pSSAOStageLayer->setCamera(pCamera);
	pSSAOStageLayer->setRenderTarget(m_pResultRenderTexture);
	pSSAOStageLayer->setIsRefreshRenderTarget(CWTRUE);
	pSSAOStageLayer->setIsClearColor(CWTRUE);
	pSSAOStageLayer->setIsClearDepth(CWFALSE);
	pSSAOStageLayer->setIsClearStencil(CWFALSE);
	pSSAOStageLayer->setUniformEffect(pSSAOEffect);
	pSSAOStageLayer->addSelfRenderNode(m_pScreenQuad);
	pSSAOStageLayer->setFilterType(eStageLayerFliterSelf);

	this->addChildPipelineNode(pSSAOStageLayer);

	return CWTRUE;
}

CWBOOL cwSSAOStage::buildBlurTexture()
{
	m_pBlurTexture = cwRepertory::getInstance().getDevice()->createRTTexture(
		m_pResultRenderTexture->getWidth(),
		m_pResultRenderTexture->getHeight(),
		m_pResultRenderTexture->getFormat(),
		CWTRUE
		);
	if (!m_pBlurTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pBlurTexture);

	return CWTRUE;
}

CWBOOL cwSSAOStage::buildHorizBlurLayer()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwShader* pSSAOBlurShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_SSAO_SHADER_FILE);
	if (!pSSAOBlurShader) return CWFALSE;

	cwEffect* pSSAOHorizBlurEffect = cwEffect::create();
	if (!pSSAOHorizBlurEffect) return CWFALSE;
	pSSAOHorizBlurEffect->setShader(pSSAOBlurShader);
	pSSAOHorizBlurEffect->setTech(CW_SSAO_SHADER_HORIZ_BLUR_TECH);

	cwEffectTextureParameter* pTexParameter = cwEffectTextureParameter::create();
	if (!pTexParameter) return CWFALSE;
	pTexParameter->setParameterName("gSSAOBlurTexture");
	pTexParameter->setTexture(m_pResultRenderTexture);
	pSSAOHorizBlurEffect->addParameter(pTexParameter);

	cwStageLayer* pHorizBlurLayer = cwStageLayer::create();
	if (!pHorizBlurLayer) return CWFALSE;

	pHorizBlurLayer->setName(CW_SSAO_HORIZ_BLUR_LAYER_NAME);
	pHorizBlurLayer->setCamera(pCamera);
	pHorizBlurLayer->setRenderTarget(m_pBlurTexture);
	pHorizBlurLayer->setIsRefreshRenderTarget(CWTRUE);
	pHorizBlurLayer->setIsClearColor(CWTRUE);
	pHorizBlurLayer->setIsClearDepth(CWFALSE);
	pHorizBlurLayer->setIsClearStencil(CWFALSE);
	pHorizBlurLayer->setUniformEffect(pSSAOHorizBlurEffect);
	pHorizBlurLayer->addSelfRenderNode(m_pScreenQuad);
	pHorizBlurLayer->setFilterType(eStageLayerFliterSelf);

	this->addChildPipelineNode(pHorizBlurLayer);

	return CWTRUE;
}

CWBOOL cwSSAOStage::buildVertBlurLayer()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwShader* pSSAOBlurShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_SSAO_SHADER_FILE);
	if (!pSSAOBlurShader) return CWFALSE;

	cwEffect* pSSAOVertBlurEffect = cwEffect::create();
	if (!pSSAOVertBlurEffect) return CWFALSE;
	pSSAOVertBlurEffect->setShader(pSSAOBlurShader);
	pSSAOVertBlurEffect->setTech(CW_SSAO_SHADER_VERT_BLUR_TECH);

	cwEffectTextureParameter* pTexParameter = cwEffectTextureParameter::create();
	if (!pTexParameter) return CWFALSE;
	pTexParameter->setParameterName("gSSAOBlurTexture");
	pTexParameter->setTexture(m_pBlurTexture);
	pSSAOVertBlurEffect->addParameter(pTexParameter);

	cwStageLayer* pVertBlurLayer = cwStageLayer::create();
	if (!pVertBlurLayer) return CWFALSE;

	pVertBlurLayer->setName(CW_SSAO_HORIZ_BLUR_LAYER_NAME);
	pVertBlurLayer->setCamera(pCamera);
	pVertBlurLayer->setRenderTarget(m_pResultRenderTexture);
	pVertBlurLayer->setIsRefreshRenderTarget(CWTRUE);
	pVertBlurLayer->setIsClearColor(CWTRUE);
	pVertBlurLayer->setIsClearDepth(CWFALSE);
	pVertBlurLayer->setIsClearStencil(CWFALSE);
	pVertBlurLayer->setUniformEffect(pSSAOVertBlurEffect);
	pVertBlurLayer->addSelfRenderNode(m_pScreenQuad);
	pVertBlurLayer->setFilterType(eStageLayerFliterSelf);

	this->addChildPipelineNode(pVertBlurLayer);

	return CWTRUE;
}

CWVOID cwSSAOStage::showResult(const cwVector2D& pos, const cwVector2D& scale)
{
	cwStage::showResult(pos, scale);

	if (!m_pSSAOSprite) {
		cwShader* pShader = cwRepertory::getInstance().getShaderManager()->createShader("SpriteRenderTechnique.hlsl");
		cwEffect* pSpriteEffect = cwEffect::create();

		if (!pShader || !pSpriteEffect) return;

		pSpriteEffect->setShader(pShader);
		pSpriteEffect->setTech("SpriteRenderTech");

		m_pSSAOSprite = this->createResultSprite(this->getRenderResult(), pSpriteEffect);
		CW_SAFE_RETAIN(m_pSSAOSprite);
	}
}

CWVOID cwSSAOStage::bindingResultParameter(cwShader* pShader)
{
	if (pShader) {
		pShader->setVariableTexture(CW_SSAO_PARAM_TEXTURE, m_pResultRenderTexture);
	}
}

CWVOID cwSSAOStage::showResult()
{
	if (m_pSSAOSprite && !m_pSSAOSprite->getParent()) {
		cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
		pCurrScene->addChild2D(m_pSSAOSprite);
	}
}

NS_MINIR_END
