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

#include "cwRenderer.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Entity/cwScene.h"
#include "Entity/cwEntity.h"
#include "Engine/cwEngine.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Material/cwMaterial.h"
#include "Effect/cwEffect.h"
#include "effect/Parameter/cwEffectTextureParameter.h"
#include "Stage/cwStage.h"
#include "Stage/cwStageLayer.h"
#include "2D/cw2DStageLayer.h"
#include "Generator/cwRenderObjectGenerator.hpp"
#include "Camera/cwCamera.h"

#include "utlist.h"

NS_MINIR_BEGIN

cwRenderer* cwRenderer::create()
{
	cwRenderer* pRenderer = new cwRenderer();
	if (pRenderer && pRenderer->init()) {
		pRenderer->autorelease();
		return pRenderer;
	}

	CW_SAFE_DELETE(pRenderer);
	return nullptr;
}

cwRenderer::cwRenderer():
m_iListPoolIndex(0),
m_pRenderListHead(nullptr),
m_pCurrRenderStage(nullptr),
m_pFinalStage(nullptr),
m_pTexFinalRenderTarget(nullptr),
m_p2DStage(nullptr)
{

}

cwRenderer::~cwRenderer()
{
	m_nVecStage.clear();
	CW_SAFE_RELEASE_NULL(m_pFinalStage);
	CW_SAFE_RELEASE_NULL(m_pTexFinalRenderTarget);
	CW_SAFE_RELEASE_NULL(m_p2DStage);
	m_pRenderListHead = nullptr;
	m_pCurrRenderStage = nullptr;
}

CWBOOL cwRenderer::init()
{
	if (!buildFinalRenderTarget()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwRenderer::buildFinalRenderTarget()
{
	cwRepertory& repertory = cwRepertory::getInstance();
	m_pTexFinalRenderTarget = repertory.getDevice()->createRTTexture();
	if (!m_pTexFinalRenderTarget) return CWFALSE;
	CW_SAFE_RETAIN(m_pTexFinalRenderTarget);

	return CWTRUE;
}

CWBOOL cwRenderer::buildFinalStage()
{
	if (m_nVecStage.empty()) return CWFALSE;
	cwStage* pLastStage = m_nVecStage.back();

	{
		m_pFinalStage = cwStage::create(m_pTexFinalRenderTarget);
		if (!m_pFinalStage) return CWFALSE;
		CW_SAFE_RETAIN(m_pFinalStage);
		m_pFinalStage->setName("FinalStage");
	}

	cwRepertory& repertory = cwRepertory::getInstance();
	cwShader* pSpriteShader = repertory.getShaderManager()->createShader("SpriteRenderTechnique.hlsl");
	if (!pSpriteShader) return CWFALSE;

	cwEffect* pSpriteEffect = cwEffect::create();
	pSpriteEffect->setShader(pSpriteShader);
	pSpriteEffect->setTech("SpriteRenderTech");

	{
		cwEffectTextureParameter* pTexParameter = cwEffectTextureParameter::create();
		pTexParameter->setParameterName("gSpriteTexture");
		pTexParameter->setTexture(pLastStage->getRenderTarget());
		pSpriteEffect->addParameter(pTexParameter);
	}

	cwRenderObjectGenerator<cwVertexPosTex> generator;
	cwRenderObject* pScreenObject = generator.generateQuad("PosTex");
	cwEntity* pScreenQuad = cwEntity::create();
	pScreenQuad->setRenderObject(pScreenObject);

	{
		cwCamera* pHomoCamera = cwRepertory::getInstance().getEngine()->getCamera("Homo");
		if (!pHomoCamera) return CWFALSE;

		cwStageLayer* pStageLayer = cwStageLayer::create();
		pStageLayer->setCamera(pHomoCamera);
		pStageLayer->setRenderTarget(m_pFinalStage->getRenderTarget());
		pStageLayer->setIsRefreshRenderTarget(CWTRUE);
		pStageLayer->setDepthStencil(nullptr);
		pStageLayer->setIsClearColor(CWFALSE);
		pStageLayer->setIsClearDepth(CWFALSE);
		pStageLayer->setIsClearStencil(CWFALSE);
		pStageLayer->setUniformEffect(pSpriteEffect);
		pStageLayer->setFilterType(eStageLayerFliterSelf);
		pStageLayer->addSelfRenderNode(pScreenQuad);

		m_pFinalStage->addChildPipelineNode(pStageLayer);
	}

	return CWTRUE;
}

CWBOOL cwRenderer::build2DStage()
{
	m_p2DStage = cwStage::create(m_pTexFinalRenderTarget);
	if (!m_p2DStage) return CWFALSE;
	CW_SAFE_RETAIN(m_p2DStage);

	cwRepertory& repertory = cwRepertory::getInstance();
	cwShader* pSpriteShader = repertory.getShaderManager()->createShader("SpriteRenderTechnique.hlsl");
	if (!pSpriteShader) return CWFALSE;

	cwEffect* pSpriteEffect = cwEffect::create();
	pSpriteEffect->setShader(pSpriteShader);
	pSpriteEffect->setTech("SpriteRenderTech");

	{
		cwCamera* pOrthoCamera = cwRepertory::getInstance().getEngine()->getCamera("Ortho");
		if (!pOrthoCamera) return CWFALSE;

		cw2DStageLayer* p2DStageLayer = cw2DStageLayer::create();
		p2DStageLayer->setCamera(pOrthoCamera);
		p2DStageLayer->setRenderTarget(m_pTexFinalRenderTarget);
		p2DStageLayer->setIsRefreshRenderTarget(CWTRUE);
		p2DStageLayer->setDepthStencil(nullptr);
		p2DStageLayer->setIsClearColor(CWFALSE);
		p2DStageLayer->setIsClearDepth(CWFALSE);
		p2DStageLayer->setIsClearStencil(CWFALSE);
		p2DStageLayer->setUniformEffect(pSpriteEffect);
		p2DStageLayer->setFilterType(eStageLayerFliter2D);

		m_pFinalStage->addChildPipelineNode(p2DStageLayer);
	}

	return CWTRUE;
}

CWVOID cwRenderer::resize()
{
	//CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	//CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	//if (m_pViewPort) {
	//	m_pViewPort->refresh(
	//		m_fViewPortTopLeftX*(CWFLOAT)winWidth, m_fViewPortTopLeftY*(CWFLOAT)winHeight,
	//		m_fViewPortWidth*(CWFLOAT)winWidth, m_fViewPortHeight*(CWFLOAT)winHeight,
	//		m_fViewPortMinDepth, m_fViewPortMaxDepth);
	//}
}

CWVOID cwRenderer::addStage(cwStage* pStage)
{
	m_nVecStage.pushBack(pStage);
}

CWVOID cwRenderer::addStageRealTime(cwStage* pStage)
{
	if (!pStage) return;
	if (!m_pCurrRenderStage) return;

	cwRenderer::sRendererListNode* pNode = nullptr;
	DL_SEARCH_SCALAR(m_pRenderListHead, pNode, m_pStage, m_pCurrRenderStage);
	if (pNode) {
		sRendererListNode* pNewNode = getAvaiableListNode();
		if (pNewNode) {
			pNewNode->m_pStage = pStage;

			if (pNode->next)
				DL_PREPEND_ELEM(m_pRenderListHead, pNode->next, pNewNode);
			else
				DL_APPEND(m_pRenderListHead, pNewNode);
		}
	}
}

CWVOID cwRenderer::begin()
{
	m_pCurrRenderStage = nullptr;

	m_pRenderListHead = buildStageList();
}

CWVOID cwRenderer::render()
{
	if (m_pRenderListHead) {
		sRendererListNode* pElement = nullptr;

		DL_FOREACH(m_pRenderListHead, pElement) {
			m_pCurrRenderStage = pElement->m_pStage;
			if (m_pCurrRenderStage)
				m_pCurrRenderStage->doRender();
		}
	}
}

CWVOID cwRenderer::end()
{
	cwRepertory::getInstance().getDevice()->swap();
}

cwStage* cwRenderer::getStage(const CWSTRING& strName)
{
	for (auto pStage : m_nVecStage) {
		if (pStage->name() == strName) return pStage;
	}

	return nullptr;
}

cwRenderer::sRendererListNode* cwRenderer::getAvaiableListNode()
{
	if (m_iListPoolIndex >= CW_RENDERER_LIST_POOL_SIZE) return nullptr;
	return &(m_nListNodePool[m_iListPoolIndex++]);
}

cwRenderer::sRendererListNode* cwRenderer::buildStageList()
{
	m_iListPoolIndex = 0;
	sRendererListNode* pHead = nullptr;
	for (auto pStage : m_nVecStage) {
		if (pStage->isEnable()) {
			sRendererListNode* pNode = getAvaiableListNode();
			if (pNode) {
				pNode->m_pStage = pStage;
				DL_APPEND(pHead, pNode);
			}
		}
	}

	if (!m_pFinalStage)
		buildFinalStage();

	if (!m_p2DStage)
		build2DStage();

	if (m_pFinalStage) {
		sRendererListNode* pNode = getAvaiableListNode();
		if (pNode) {
			pNode->m_pStage = m_pFinalStage;
			DL_APPEND(pHead, pNode);
		}
	}

	if (m_p2DStage) {
		sRendererListNode* pNode = getAvaiableListNode();
		if (pNode) {
			pNode->m_pStage = m_p2DStage;
			DL_APPEND(pHead, pNode);
		}
	}

	return pHead;
}

cwRay cwRenderer::getPickingRayWorld(CWFLOAT fPosX, CWFLOAT fPosY)
{
	cwCamera* pCamera = cwRepertory::getInstance().getEngine()->getCamera("Default");
	cwRay ray;

	const cwMatrix4X4& matProj = pCamera->getProjMatrix();
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	CWFLOAT fViewPortLeftX = fPosX - (CWFLOAT)winWidth;
	CWFLOAT fViewPortLeftY = fPosY - (CWFLOAT)winHeight;

	CWFLOAT fViewWidth  = (CWFLOAT)winWidth;
	CWFLOAT fViewHeight = (CWFLOAT)winHeight;

	CWFLOAT vx = (+2.0f * fViewPortLeftX / fViewWidth - 1.0f) / matProj.m11;
	CWFLOAT vy = (-2.0f * fViewPortLeftY / fViewHeight + 1.0f) / matProj.m22;

	const cwMatrix4X4& matView = pCamera->getViewMatrix();
	if (matView.inverseExist()) {
		cwMatrix4X4 matInvView = matView.inverse();

		ray.m_nOrigin = cwVector4D(0.0f, 0.0f, 0.0f, 1.0f) * matInvView;
		ray.m_nDir = cwVector3D(vx, vy, 1.0f) * matInvView;
		ray.m_nDir.normalize();
	}

	return ray;
}

cwRay cwRenderer::getPickingRayWorld(cwTouch* pTouch)
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	CWFLOAT fX = pTouch->getScreenPos().x;
	CWFLOAT fY = pTouch->getScreenPos().y;

	fY = winHeight - fY;
	return getPickingRayWorld(fX, fY);
}

NS_MINIR_END
