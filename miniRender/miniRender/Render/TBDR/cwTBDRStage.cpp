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

#include "cwTBDRStage.h"
#include "cwTBDRCoreStageLayer.h"
#include "Render/Stage/cwStageLayer.h"
#include "Texture/cwTexture.h"
#include "Texture/cwMultiRTTexture.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Shader/cwShaderManager.h"
#include "Shader/cwShader.h"
#include "effect/cwEffect.h"
#include "effect/cwGPEffect.h"
#include "effect/Parameter/cwEffectTextureParameter.h"
#include "effect/Parameter/cwEffectBufferParameter.h"
#include "effect/Parameter/cwEffectDataParameter.h"
#include "Engine/cwEngine.h"
#include "Generator/cwGeometryGenerator.h"
#include "Generator/cwRenderObjectGenerator.hpp"
#include "Entity/cwEntity.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "Texture/cwTextureManager.h"

NS_MINIR_BEGIN

cwTBDRStage::cwTBDRStage() :
m_pGBuffer(nullptr),
m_pDepthStencil(nullptr),
m_pScreenQuad(nullptr),
m_pLitTexture(nullptr),
//m_pFinalRenderTarget(nullptr),
m_pParamFrameDim(nullptr)
{

}

cwTBDRStage::~cwTBDRStage()
{
	CW_SAFE_RELEASE_NULL(m_pGBuffer);
	CW_SAFE_RELEASE_NULL(m_pDepthStencil);
	CW_SAFE_RELEASE_NULL(m_pScreenQuad);
	CW_SAFE_RELEASE_NULL(m_pLitTexture);
	//CW_SAFE_RELEASE_NULL(m_pFinalRenderTarget);
	CW_SAFE_RELEASE_NULL(m_pParamFrameDim);
}

CWBOOL cwTBDRStage::init()
{
	if (!cwStage::init()) return CWFALSE;

	if (!buildParam()) return CWFALSE;
	if (!buildGBuffer()) return CWFALSE;
	if (!buildDepthStencil()) return CWFALSE;
	if (!buildLitTexture()) return CWFALSE;
	//if (!buildFinalRenderTarget()) return CWFALSE;
	if (!buildEntity()) return CWFALSE;

	if (!buildClearStageLayer()) return CWFALSE;
	if (!buildRenderScreenStageLayer()) return CWFALSE;
	if (!buildTBDRCoreStageLayer()) return CWFALSE;
	if (!buildResultStageLayer()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildParam()
{
	CWUINT dim[4] = { 0, 0, 0, 0 };
	dim[0] = cwRepertory::getInstance().getUInt(gValueWinWidth);
	dim[1] = cwRepertory::getInstance().getUInt(gValueWinHeight);

	cwEffectDataParameter* pDataParameter = cwEffectDataParameter::create();
	if (!pDataParameter) return CWFALSE;
	pDataParameter->setData(dim, sizeof(dim));
	pDataParameter->setParameterName(CW_TBDR_SHADER_PARAM_FRAME_DIM);

	m_pParamFrameDim = pDataParameter;
	CW_SAFE_RETAIN(m_pParamFrameDim);

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildGBuffer()
{
	CWFLOAT winWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));

	m_pGBuffer = cwMultiRTTexture::create(winWidth, winHeight);
	if (!m_pGBuffer) return CWFALSE;

#if CW_ENABLE_MSAA
	if (!m_pGBuffer->addRTTexture(eFormatR16g16Float, CW_MSAA_SAMPLES, CWFALSE)) return CWFALSE;
	if (!m_pGBuffer->addRTTexture(eFormatR8g8b8a8Unorm, CW_MSAA_SAMPLES, CWFALSE)) return CWFALSE;
	if (!m_pGBuffer->addRTTexture(eFormatR8g8b8a8Unorm, CW_MSAA_SAMPLES, CWFALSE)) return CWFALSE;
#else
	if (!m_pGBuffer->addRTTexture(eFormatR16g16Float)) return CWFALSE;
	if (!m_pGBuffer->addRTTexture(eFormatR8g8b8a8Unorm)) return CWFALSE;
	if (!m_pGBuffer->addRTTexture(eFormatR8g8b8a8Unorm)) return CWFALSE;
#endif

	CW_SAFE_RETAIN(m_pGBuffer);
	return CWTRUE;
}

CWBOOL cwTBDRStage::buildDepthStencil()
{
	CWFLOAT winWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));

#if CW_ENABLE_MSAA
	m_pDepthStencil = cwRepertory::getInstance().getDevice()->createDSTexture(winWidth, winHeight, CW_MSAA_SAMPLES, CWTRUE, CWFALSE);
#else
	m_pDepthStencil = cwRepertory::getInstance().getDevice()->createDSTexture(winWidth, winHeight, CWTRUE);
#endif

	if (!m_pDepthStencil) return CWFALSE;
	CW_SAFE_RETAIN(m_pDepthStencil);

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildLitTexture()
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

#if CW_ENABLE_MSAA
	CWUINT uPixelCnt = winWidth*winHeight*CW_MSAA_SAMPLES;
	m_pLitTexture = cwRepertory::getInstance().getDevice()->createShaderBuffer(nullptr, sizeof(sLitBuffer), uPixelCnt, CWTRUE);
#else
	m_pLitTexture = cwRepertory::getInstance().getDevice()->createRWTexture(winWidth, winHeight, eFormatR8g8b8a8Unorm);
#endif

	if (!m_pLitTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pLitTexture);

	return CWTRUE;
}

//CWBOOL cwTBDRStage::buildFinalRenderTarget()
//{
//	CWFLOAT winWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
//	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));
//
//	m_pFinalRenderTarget = cwRepertory::getInstance().getDevice()->createRTTexture();
//	if (!m_pFinalRenderTarget) return CWFALSE;
//	CW_SAFE_RETAIN(m_pFinalRenderTarget);
//
//	return CWTRUE;
//}

CWBOOL cwTBDRStage::buildEntity()
{
	cwRenderObjectGenerator<cwVertexPosTex> generator;
	cwRenderObject* pRenderObj = generator.generateQuad("PosTex");

	m_pScreenQuad = cwEntity::create();
	if (!m_pScreenQuad) return CWFALSE;
	m_pScreenQuad->setRenderObject(pRenderObj);
	CW_SAFE_RETAIN(m_pScreenQuad);

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildClearStageLayer()
{
	cwCamera* pHomoCamera = cwRepertory::getInstance().getEngine()->getCamera("Homo");
	if (!pHomoCamera) return CWFALSE;

	cwShader* pTBDRShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_TBDR_SHADER_FILE);
	if (!pTBDRShader) return CWFALSE;

	cwEffect* pEffectClearGBuffer = cwEffect::create();
	if (!pEffectClearGBuffer) return CWFALSE;
	pEffectClearGBuffer->setShader(pTBDRShader);
	pEffectClearGBuffer->setTech(CW_TBDR_SHADER_TECH_CLEAR);

	cwStageLayer* pClearStageLayer = cwStageLayer::create();
	pClearStageLayer->setName(CW_TBDR_CLEAR_LAYER_NAME);
	pClearStageLayer->setCamera(pHomoCamera);
	pClearStageLayer->setRenderTarget(m_pGBuffer);
	pClearStageLayer->setIsRefreshRenderTarget(CWTRUE);
	pClearStageLayer->setIsClearColor(CWFALSE);
	pClearStageLayer->setIsClearDepth(CWFALSE);
	pClearStageLayer->setIsClearStencil(CWFALSE);
	pClearStageLayer->setUniformEffect(pEffectClearGBuffer);
	pClearStageLayer->setFilterType(eStageLayerFliterSelf);
	pClearStageLayer->addSelfRenderNode(m_pScreenQuad);

	this->addStageLayer(pClearStageLayer);

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildRenderScreenStageLayer()
{
	cwShader* pTBDRShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_TBDR_SHADER_FILE);
	if (!pTBDRShader) return CWFALSE;

	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pTBDRShader);
	pEffect->setTech(CW_TBDR_SHADER_TECH_BUILD);

	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwStageLayer* pRenderStageLayer = cwStageLayer::create();
	pRenderStageLayer->setName(CW_TBDR_RENDER_LAYER_NAME);
	pRenderStageLayer->setCamera(pCamera);
	pRenderStageLayer->setRenderTarget(m_pGBuffer);
	pRenderStageLayer->setDepthStencil(m_pDepthStencil);
	pRenderStageLayer->setIsRefreshRenderTarget(CWTRUE);
	pRenderStageLayer->setIsClearColor(CWFALSE);
	pRenderStageLayer->setIsClearDepth(CWTRUE);
	pRenderStageLayer->setIsClearStencil(CWTRUE);
	pRenderStageLayer->setFilterType(eStageLayerFliterScene);
	pRenderStageLayer->setUniformEffect(pEffect);

	this->addStageLayer(pRenderStageLayer);

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildTBDRCoreStageLayer()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwShader* pTBDRCoreShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_TBDR_CORE_SHADER_FILE);
	if (!pTBDRCoreShader) return CWFALSE;

	cwGPEffect* pTBDRCoreEffect = cwGPEffect::create();
	if (!pTBDRCoreEffect) return CWFALSE;

	CWUINT iGBufferWidth = static_cast<CWUINT>(m_pGBuffer->getWidth());
	CWUINT iGBufferHeight = static_cast<CWUINT>(m_pGBuffer->getHeight());
	pTBDRCoreEffect->getGPInfo().groupX = (iGBufferWidth + 16 - 1) / 16;
	pTBDRCoreEffect->getGPInfo().groupY = (iGBufferHeight + 16 - 1) / 16;
	pTBDRCoreEffect->getGPInfo().groupZ = 1;

	pTBDRCoreEffect->setShader(pTBDRCoreShader);
	pTBDRCoreEffect->setTech(CW_TBDR_SHADER_TECH_CORE);

	cwEffectTextureParameter* pTexNormalParam = cwEffectTextureParameter::create();
	pTexNormalParam->setTexture(m_pGBuffer->getTexture(0));
	pTexNormalParam->setParameterName(CW_TBDR_SHADER_PARAM_NORMAL_TEXTURE);
	pTBDRCoreEffect->addParameter(pTexNormalParam);

	cwEffectTextureParameter* pTexDiffuseParam = cwEffectTextureParameter::create();
	pTexDiffuseParam->setTexture(m_pGBuffer->getTexture(1));
	pTexDiffuseParam->setParameterName(CW_TBDR_SHADER_PARAM_DIFFUSE_TEXTURE);
	pTBDRCoreEffect->addParameter(pTexDiffuseParam);

	cwEffectTextureParameter* pTexSpecularParam = cwEffectTextureParameter::create();
	pTexSpecularParam->setTexture(m_pGBuffer->getTexture(2));
	pTexSpecularParam->setParameterName(CW_TBDR_SHADER_PARAM_SPECULAR_TEXTURE);
	pTBDRCoreEffect->addParameter(pTexSpecularParam);

	cwEffectTextureParameter* pTexDepthParam = cwEffectTextureParameter::create();
	pTexDepthParam->setTexture(m_pDepthStencil);
	pTexDepthParam->setParameterName(CW_TBDR_SHADER_PARAM_DEPTH_TEXTURE);
	pTBDRCoreEffect->addParameter(pTexDepthParam);

	pTBDRCoreEffect->addParameter(m_pParamFrameDim);

#if CW_ENABLE_MSAA
	cwEffectBufferParameter* pTexBufferParam = cwEffectBufferParameter::create();
	pTexBufferParam->setBuffer(m_pLitTexture);
	pTexBufferParam->setWritable(CWTRUE);
	pTexBufferParam->setParameterName(CW_TBDR_SHADER_PARAM_FRAME_BUFFER);
	pTBDRCoreEffect->addParameter(pTexBufferParam);
#else
	cwEffectTextureParameter* pTexLitParam = cwEffectTextureParameter::create();
	pTexLitParam->setTexture(m_pLitTexture);
	pTexLitParam->setParameterName("gLitTexture");
	pTexLitParam->setWritable(CWTRUE);
	pTBDRCoreEffect->addParameter(pTexLitParam);
#endif

	cwTBDRCoreStageLayer* pTBDRCoreStageLayer = cwTBDRCoreStageLayer::create();
	pTBDRCoreStageLayer->setName(CW_TBDR_CORE_LAYER_NAME);
	pTBDRCoreStageLayer->setCamera(pCamera);
	pTBDRCoreStageLayer->setIsRefreshRenderTarget(CWTRUE);
	pTBDRCoreStageLayer->setIsClearColor(CWFALSE);
	pTBDRCoreStageLayer->setIsClearDepth(CWFALSE);
	pTBDRCoreStageLayer->setIsClearStencil(CWFALSE);
	pTBDRCoreStageLayer->addSelfRenderNode(m_pScreenQuad);
	pTBDRCoreStageLayer->setFilterType(eStageLayerFliterSelf);
	pTBDRCoreStageLayer->setUniformEffect(pTBDRCoreEffect);

	this->addStageLayer(pTBDRCoreStageLayer);

	return CWTRUE;
}

CWBOOL cwTBDRStage::buildResultStageLayer()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Homo");
	if (!pCamera) return CWFALSE;

	cwShader* pTBDRShader = cwRepertory::getInstance().getShaderManager()->createShader(CW_TBDR_SHADER_FILE);
	if (!pTBDRShader) return CWFALSE;

	cwEffect* pResultEffect = cwEffect::create();
	pResultEffect->setShader(pTBDRShader);
	pResultEffect->setTech(CW_TBDR_SHADER_TECH_RESULT);

	pResultEffect->addParameter(m_pParamFrameDim);

#if CW_ENABLE_MSAA
	cwEffectBufferParameter* pTexBufferParam = cwEffectBufferParameter::create();
	pTexBufferParam->setBuffer(m_pLitTexture);
	pTexBufferParam->setParameterName(CW_TBDR_SHADER_PARAM_LIT_FRAME_BUFFER);
	pResultEffect->addParameter(pTexBufferParam);
#else
	cwEffectTextureParameter* pTexParam = cwEffectTextureParameter::create();
	pTexParam->setTexture(m_pLitTexture);
	pTexParam->setParameterName("gQuadTexture");
	pResultEffect->addParameter(pTexParam);
#endif

	cwStageLayer* pResultStageLayer = cwStageLayer::create();
	if (!pResultStageLayer) return CWFALSE;

	pResultStageLayer->setName(CW_TBDR_RESULT_LAYER_NAME);
	pResultStageLayer->setCamera(pCamera);
	pResultStageLayer->setRenderTarget(m_pResultRenderTexture);
	pResultStageLayer->setIsRefreshRenderTarget(CWTRUE);
	pResultStageLayer->setIsClearColor(CWFALSE);
	pResultStageLayer->setIsClearDepth(CWFALSE);
	pResultStageLayer->setIsClearStencil(CWFALSE);
	pResultStageLayer->addSelfRenderNode(m_pScreenQuad);
	pResultStageLayer->setFilterType(eStageLayerFliterSelf);
	pResultStageLayer->setUniformEffect(pResultEffect);

	this->addStageLayer(pResultStageLayer);

	return CWTRUE;
}

NS_MINIR_END
