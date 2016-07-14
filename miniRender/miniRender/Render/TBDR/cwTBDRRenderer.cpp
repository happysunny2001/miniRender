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

#include "cwTBDRRenderer.h"
#include "Texture/cwTexture.h"
#include "Texture/cwMultiRTTexture.h"
#include "Texture/cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "effect/cwEffect.h"
#include "effect/cwGPEffect.h"
#include "effect/cwEffectTextureParameter.h"
#include "Entity/cwEntity.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Generator/cwGeometryGenerator.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "../Stage/cwStage.h"
#include "../Stage/Layer/cwStageLayer.h"
#include "../Stage/cwSpriteStage.h"
#include "Engine/cwEngine.h"
#include "Buffer/cwBuffer.h"
#include "Entity/cwScene.h"
#include "Camera/cwCamera.h"

NS_MINIR_BEGIN

cwTBDRRenderer* cwTBDRRenderer::create()
{
	cwTBDRRenderer* pRenderer = new cwTBDRRenderer();
	if (pRenderer && pRenderer->init()) {
		pRenderer->autorelease();
		return pRenderer;
	}

	CW_SAFE_DELETE(pRenderer);
	return nullptr;
}

cwTBDRRenderer::cwTBDRRenderer() : 
m_pGBuffer(nullptr),
m_pDepthStencil(nullptr),
m_pLitTexture(nullptr),
m_pFinalRenderTarget(nullptr),
m_pClearScreenQuad(nullptr),
m_pEffectClearGBuffer(nullptr),
m_pCoreScreenQuad(nullptr),
m_pEffectTBDRCore(nullptr),
m_pResultScreenQuad(nullptr),
m_pEffectResult(nullptr),
m_pPointLightBuffer(nullptr),
m_pTBDRCoreStage(nullptr)
{

}

cwTBDRRenderer::~cwTBDRRenderer()
{
	CW_SAFE_RELEASE_NULL(m_pGBuffer);
	CW_SAFE_RELEASE_NULL(m_pDepthStencil);
	CW_SAFE_RELEASE_NULL(m_pLitTexture);
	CW_SAFE_RELEASE_NULL(m_pFinalRenderTarget);
	CW_SAFE_RELEASE_NULL(m_pClearScreenQuad);
	CW_SAFE_RELEASE_NULL(m_pEffectClearGBuffer);
	CW_SAFE_RELEASE_NULL(m_pCoreScreenQuad);
	CW_SAFE_RELEASE_NULL(m_pEffectTBDRCore);
	CW_SAFE_RELEASE_NULL(m_pResultScreenQuad);
	CW_SAFE_RELEASE_NULL(m_pEffectResult);
	CW_SAFE_RELEASE_NULL(m_pPointLightBuffer);
	CW_SAFE_RELEASE_NULL(m_pTBDRCoreStage);
}

CWBOOL cwTBDRRenderer::init()
{
	if (!cwRenderer::init()) return CWFALSE;

	if (!buildGBuffer()) return CWFALSE;
	if (!buildDepthStencil()) return CWFALSE; 
	if (!buildLitTexture()) return CWFALSE;
	if (!buildFinalRenderTarget()) return CWFALSE;
	if (!buildPointLightBuffer()) return CWFALSE;

	if (!buildEffect()) return CWFALSE;
	if (!buildEntity()) return CWFALSE;

	if (!buildClearStage()) return CWFALSE;
	if (!buildRenderScreenStage()) return CWFALSE;
	if (!buildTBDRCoreStage()) return CWFALSE;
	if (!buildResultStage()) return CWFALSE;
	if (!buildSpriteStage(CWTRUE)) return CWFALSE;

	this->createViewPort(0, 0, 1.0, 1.0, 0, 1.0);

	return CWTRUE;
}

CWVOID cwTBDRRenderer::configLight()
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene || !m_pCurrCamera || !m_pCurrShader) return;
	if (m_pCurrRenderStage != m_pTBDRCoreStage) return;

	const cwMatrix4X4& matView = m_pCurrCamera->getViewMatrix();
	const cwVector<cwPointLight*>& vecLight = pCurrScene->getPointLights();

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it) {
		m_nVecViewSpacePointLights[index] = (*it)->getData();
		cwVector4D v(m_nVecViewSpacePointLights[index].m_nPosition, 1.0f);
		m_nVecViewSpacePointLights[index].m_nPosition = v*matView;

		index++;
		if (index >= MAX_LIGHTS) break;
	}

	m_pPointLightBuffer->refresh(&(m_nVecViewSpacePointLights[0]));
	m_pCurrShader->setVariableBuffer("gPointLights", m_pPointLightBuffer);
	m_pCurrShader->setVariableInt("gPointLightCnt", index);
}

CWVOID cwTBDRRenderer::perFrameConfig()
{
	cwRenderer::perFrameConfig();

	if (m_pCurrShader) {
		CWUINT dim[4] = {0, 0, 0, 0};
		dim[0] = static_cast<CWUINT>(m_pGBuffer->getWidth());
		dim[1] = static_cast<CWUINT>(m_pGBuffer->getHeight());

		m_pCurrShader->setVariableData("gFrameBufferDimensions", dim, 0, sizeof(dim));
	}
}

CWBOOL cwTBDRRenderer::buildGBuffer()
{
	CWFLOAT winWidth  = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));

	m_pGBuffer = cwMultiRTTexture::create(winWidth, winHeight);
	if (!m_pGBuffer) return CWFALSE;

	if (!m_pGBuffer->addRTTexture(eFormatR16g16Float)) return CWFALSE;
	if (!m_pGBuffer->addRTTexture(eFormatR8g8b8a8Unorm)) return CWFALSE;
	if (!m_pGBuffer->addRTTexture(eFormatR8g8b8a8Unorm)) return CWFALSE;
	CW_SAFE_RETAIN(m_pGBuffer);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildDepthStencil()
{
	CWFLOAT winWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));

	m_pDepthStencil = cwRepertory::getInstance().getDevice()->createDSTexture(winWidth, winHeight, CWTRUE);
	if (!m_pDepthStencil) return CWFALSE;
	CW_SAFE_RETAIN(m_pDepthStencil);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildLitTexture()
{
	CWFLOAT winWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));

	m_pLitTexture = cwRepertory::getInstance().getDevice()->createRWTexture(winWidth, winHeight, eFormatR8g8b8a8Unorm);
	if (!m_pLitTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pLitTexture);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildFinalRenderTarget()
{
	CWFLOAT winWidth = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinWidth));
	CWFLOAT winHeight = static_cast<CWFLOAT>(cwRepertory::getInstance().getUInt(gValueWinHeight));

	m_pFinalRenderTarget = cwRepertory::getInstance().getDevice()->createRTTexture();
	if (!m_pFinalRenderTarget) return CWFALSE;
	CW_SAFE_RETAIN(m_pFinalRenderTarget);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildEffect()
{
	cwShader* pTBDRShader = cwRepertory::getInstance().getShaderManager()->createShader("TBDR/TBDRTechnique.hlsl");
	cwShader* pTBDRCoreShader = cwRepertory::getInstance().getShaderManager()->createShader("TBDR/TBDRCoreTechnique.hlsl");
	if (!pTBDRShader || !pTBDRCoreShader) return CWFALSE;

	{
		m_pEffectClearGBuffer = cwEffect::create();
		if (!m_pEffectClearGBuffer) return CWFALSE;
		m_pEffectClearGBuffer->setShader(pTBDRShader);
		m_pEffectClearGBuffer->setTech("GBufferClear");
		CW_SAFE_RETAIN(m_pEffectClearGBuffer);
	}

	{
		m_pEffectTBDRCore = cwGPEffect::create();
		if (!m_pEffectTBDRCore) return CWFALSE;

		CWUINT iGBufferWidth = static_cast<CWUINT>(m_pGBuffer->getWidth());
		CWUINT iGBufferHeight = static_cast<CWUINT>(m_pGBuffer->getHeight());
		m_pEffectTBDRCore->getGPInfo().groupX = (iGBufferWidth + 16 - 1) / 16;
		m_pEffectTBDRCore->getGPInfo().groupY = (iGBufferHeight + 16 - 1) / 16;
		m_pEffectTBDRCore->getGPInfo().groupZ = 1;

		m_pEffectTBDRCore->setShader(pTBDRCoreShader);
		m_pEffectTBDRCore->setTech("TBDRCore");
		CW_SAFE_RETAIN(m_pEffectTBDRCore);

		cwEffectTextureParameter* pTexNormalParam = cwEffectTextureParameter::create();
		pTexNormalParam->setTexture(m_pGBuffer->getTexture(0));
		pTexNormalParam->setParameterName("gNormalTexture");
		m_pEffectTBDRCore->addParameter(pTexNormalParam);

		cwEffectTextureParameter* pTexDiffuseParam = cwEffectTextureParameter::create();
		pTexDiffuseParam->setTexture(m_pGBuffer->getTexture(1));
		pTexDiffuseParam->setParameterName("gDiffuseTexture");
		m_pEffectTBDRCore->addParameter(pTexDiffuseParam);

		cwEffectTextureParameter* pTexSpecularParam = cwEffectTextureParameter::create();
		pTexSpecularParam->setTexture(m_pGBuffer->getTexture(2));
		pTexSpecularParam->setParameterName("gSpecularTexture");
		m_pEffectTBDRCore->addParameter(pTexSpecularParam);

		cwEffectTextureParameter* pTexDepthParam = cwEffectTextureParameter::create();
		pTexDepthParam->setTexture(m_pDepthStencil);
		pTexDepthParam->setParameterName("gDepthTexture");
		m_pEffectTBDRCore->addParameter(pTexDepthParam);

		cwEffectTextureParameter* pTexLitParam = cwEffectTextureParameter::create();
		pTexLitParam->setTexture(m_pLitTexture);
		pTexLitParam->setParameterName("gLitTexture");
		pTexLitParam->setWritable(CWTRUE);
		m_pEffectTBDRCore->addParameter(pTexLitParam);
	}

	{
		m_pEffectResult = cwEffect::create();
		if (!m_pEffectResult) return CWFALSE;
		m_pEffectResult->setShader(pTBDRShader);
		m_pEffectResult->setTech("ScreenQuad");
		CW_SAFE_RETAIN(m_pEffectResult);

		cwEffectTextureParameter* pTexParam = cwEffectTextureParameter::create();
		pTexParam->setTexture(m_pLitTexture);
		pTexParam->setParameterName("gQuadTexture");
		m_pEffectResult->addParameter(pTexParam);
	}

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildEntity()
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateQuad(mesh);

	vector<cwVertexPosTex> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i].pos = mesh.nVertex[i].pos;
		vecVertex[i].tex = mesh.nVertex[i].tex;
	}

	cwRenderObject* pRenderObj = nullptr;
	pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(cwVertexPosTex), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), "PosTex");
	if (!pRenderObj) return CWFALSE;

	{
		m_pClearScreenQuad = cwEntity::create();
		if (!m_pClearScreenQuad) return CWFALSE;
		m_pClearScreenQuad->setRenderObject(pRenderObj);
		m_pClearScreenQuad->setEffect(m_pEffectClearGBuffer);
		CW_SAFE_RETAIN(m_pClearScreenQuad);
	}

	{
		m_pCoreScreenQuad = cwEntity::create();
		if (!m_pCoreScreenQuad) return CWFALSE;
		m_pCoreScreenQuad->setRenderObject(pRenderObj);
		m_pCoreScreenQuad->setEffect(m_pEffectTBDRCore);
		CW_SAFE_RETAIN(m_pCoreScreenQuad);
	}

	{
		m_pResultScreenQuad = cwEntity::create();
		if (!m_pResultScreenQuad) return CWFALSE;
		m_pResultScreenQuad->setRenderObject(pRenderObj);
		m_pResultScreenQuad->setEffect(m_pEffectResult);
		CW_SAFE_RETAIN(m_pResultScreenQuad);
	}

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildClearStage()
{
	cwCamera* pHomoCamera = cwRepertory::getInstance().getEngine()->getCamera("Homo");
	if (!pHomoCamera) return CWFALSE;

	cwStage* pStage = cwStage::create();
	if (!pStage) return CWFALSE;

	pStage->setName("ClearStage");
	pStage->setCamera(pHomoCamera);
	pStage->addStageEntity(m_pClearScreenQuad);
	pStage->setRenderTexture(m_pGBuffer);
	pStage->setRefreshRenderTarget(CWTRUE);
	pStage->setIsClearColor(CWFALSE);
	pStage->setIsClearDepth(CWTRUE);
	pStage->setIsClearStencil(CWTRUE);

	{
		cwStageLayer* pStageLayer = cwStageLayer::create();
		if (!pStageLayer) return CWFALSE;
		pStageLayer->setFliterType(eStageLayerFliterStage);
		pStage->addStageLayer(pStageLayer);
	}

	this->addStage(pStage);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildRenderScreenStage()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwStage* pStage = cwStage::create();
	if (!pStage) return CWFALSE;

	pStage->setName("ScreenStage");
	pStage->setCamera(pCamera);
	pStage->setRenderTexture(m_pGBuffer);
	pStage->setDepthStencil(m_pDepthStencil);
	pStage->setRefreshRenderTarget(CWTRUE);
	pStage->setIsClearColor(CWFALSE);
	pStage->setIsClearDepth(CWTRUE);
	pStage->setIsClearStencil(CWTRUE);

	{
		cwStageLayer* pStageLayer = cwStageLayer::create();
		if (!pStageLayer) return CWFALSE;
		pStageLayer->setFliterType(eStageLayerFliterEntity);
		pStage->addStageLayer(pStageLayer);
	}

	this->addStage(pStage);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildTBDRCoreStage()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	if (!pCamera) return CWFALSE;

	cwStage* pStage = cwStage::create();
	if (!pStage) return CWFALSE;

	pStage->setName("TBDRStage");
	pStage->setCamera(pCamera);
	pStage->setRenderTexture(NULL);
	pStage->setDepthStencil(NULL);
	pStage->setRefreshRenderTarget(CWTRUE);
	pStage->setIsClearColor(CWFALSE);
	pStage->setIsClearDepth(CWFALSE);
	pStage->setIsClearStencil(CWFALSE);
	pStage->addStageEntity(m_pCoreScreenQuad);

	{
		cwStageLayer* pStageLayer = cwStageLayer::create();
		if (!pStageLayer) return CWFALSE;
		pStageLayer->setFliterType(eStageLayerFliterStage);
		pStage->addStageLayer(pStageLayer);
	}

	this->addStage(pStage);

	m_pTBDRCoreStage = pStage;
	CW_SAFE_RETAIN(m_pTBDRCoreStage);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildResultStage()
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Homo");
	if (!pCamera) return CWFALSE;

	cwStage* pStage = cwStage::create();
	if (!pStage) return CWFALSE;

	pStage->setName("DebugStage");
	pStage->setCamera(pCamera);
	pStage->addStageEntity(m_pResultScreenQuad);
	pStage->setRenderTexture(m_pFinalRenderTarget);
	pStage->setRefreshRenderTarget(CWTRUE);
	pStage->setIsClearColor(CWFALSE);
	pStage->setIsClearDepth(CWTRUE);
	pStage->setIsClearStencil(CWTRUE);

	{
		cwStageLayer* pStageLayer = cwStageLayer::create();
		if (!pStageLayer) return CWFALSE;
		pStageLayer->setFliterType(eStageLayerFliterStage);
		pStageLayer->setRenderState(eRenderStateSolid);
		pStage->addStageLayer(pStageLayer);
	}

	this->addStage(pStage);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildSpriteStage(CWBOOL bEnable)
{
	cwSpriteStage* pStage = cwSpriteStage::create();
	if (!pStage) return CWFALSE;

	pStage->setEnable(bEnable);

	{
		cwStageLayer* pStageLayer = cwStageLayer::create();
		pStage->addStageLayer(pStageLayer);
	}

	this->addStage(pStage);

	return CWTRUE;
}

CWBOOL cwTBDRRenderer::buildPointLightBuffer()
{
	m_pPointLightBuffer = cwRepertory::getInstance().getDevice()->createShaderStructedBuffer(nullptr, sizeof(cwPointLight::PointLight_Struct), MAX_LIGHTS);
	if (!m_pPointLightBuffer) return CWFALSE;
	CW_SAFE_RETAIN(m_pPointLightBuffer);

	m_nVecViewSpacePointLights.resize(MAX_LIGHTS);

	return CWTRUE;
}

NS_MINIR_END
