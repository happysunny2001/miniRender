/*
Copyright © 2015 Ziwei Wang

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
#include "Entity/cwPrimitiveEntity.h"
#include "Engine/cwEngine.h"
#include "Shader/cwShader.h"
#include "Material/cwMaterial.h"
#include "Effect/cwEffect.h"
#include "Sprite/cwSpriteManager.h"
#include "Stage/cwStage.h"
#include "cwRenderBatch.h"

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
m_pCurrCamera(nullptr),
m_pCurrShader(nullptr),
m_iListPoolIndex(0),
m_pRenderListHead(nullptr),
m_pCurrRenderStage(nullptr),
m_pPrimitiveEntity(nullptr),
m_pPrimitiveBatch(nullptr),
m_pViewPort(nullptr),
m_pRendererCamera(nullptr)
{

}

cwRenderer::~cwRenderer()
{
	m_nVecStage.clear();
	CW_SAFE_RELEASE_NULL(m_pPrimitiveEntity);
	CW_SAFE_DELETE(m_pPrimitiveBatch);
	CW_SAFE_RELEASE_NULL(m_pViewPort);
	CW_SAFE_RELEASE_NULL(m_pRendererCamera);

	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;
	m_pRenderListHead = nullptr;
	m_pCurrRenderStage = nullptr;
}

CWBOOL cwRenderer::init()
{
	buildPrimitiveEntity();

	return true;
}

CWVOID cwRenderer::buildPrimitiveEntity()
{
	CW_SAFE_RELEASE_NULL(m_pPrimitiveEntity);
	m_pPrimitiveEntity = cwPrimitiveEntity::create();
	CW_SAFE_RETAIN(m_pPrimitiveEntity);

	CW_SAFE_DELETE(m_pPrimitiveBatch);
	m_pPrimitiveBatch = new cwRenderBatch();
	m_pPrimitiveBatch->m_pEffect = m_pPrimitiveEntity->getEffect();
	m_pPrimitiveBatch->m_pBlend = m_pPrimitiveEntity->getBlend();
	m_pPrimitiveBatch->m_pStencil = m_pPrimitiveEntity->getStencil();
	m_pPrimitiveBatch->m_pEntity = m_pPrimitiveEntity;
	m_pPrimitiveBatch->m_nWorldTrans = cwMatrix4X4::identityMatrix;
}

CWVOID cwRenderer::renderPrimitiveEntity()
{
	if (m_pPrimitiveBatch)
		this->render(m_pPrimitiveBatch);
}

CWVOID cwRenderer::renderPrimitive(const cwAABB& aabb)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(aabb);
	}
}

CWVOID cwRenderer::renderPrimitive(const cwAABB& aabb, const cwVector4D& color)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(aabb, color);
	}
}

CWVOID cwRenderer::renderPrimitive(const cwRay& ray)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(ray);
	}
}

CWVOID cwRenderer::renderPrimitive(const cwRay& ray, CWFLOAT fLen, const cwVector4D& color)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(ray, fLen, color);
	}
}

CWVOID cwRenderer::renderPrimitive(cwCamera* pCamera)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(pCamera);
	}
}

CWVOID cwRenderer::renderPrimitive(cwCamera* pCamera, const cwVector4D& color)
{
	if (m_pPrimitiveEntity) {
		m_pPrimitiveEntity->addPrimitive(pCamera, color);
	}
}

CWVOID cwRenderer::setCurrCamera(cwCamera* pCamera)
{
	m_pCurrCamera = pCamera;
}

CWVOID cwRenderer::setRendererCamera(cwCamera* pCamera)
{
	if (m_pRendererCamera == pCamera) return;
	CW_SAFE_RETAIN(pCamera);
	CW_SAFE_RELEASE_NULL(m_pRendererCamera);
	m_pRendererCamera = pCamera;
}

CWVOID cwRenderer::setCurrShader(cwShader* pShader)
{
	if (m_pCurrShader == pShader) return;
	m_pCurrShader = pShader;
	configLight();
}

CWVOID cwRenderer::createViewPort(CWFLOAT fTopLeftX, CWFLOAT fTopLeftY, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	m_fViewPortTopLeftX = fTopLeftX;
	m_fViewPortTopLeftY = fTopLeftY;
	m_fViewPortWidth = fWidth;
	m_fViewPortHeight = fHeight;
	m_fViewPortMinDepth = fMinDepth;
	m_fViewPortMaxDepth = fMaxDepth;

	CW_SAFE_RELEASE_NULL(m_pViewPort);

	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	m_pViewPort = cwRepertory::getInstance().getDevice()->createViewPort(
		m_fViewPortTopLeftX*(CWFLOAT)winWidth, m_fViewPortTopLeftY*(CWFLOAT)winHeight,
		m_fViewPortWidth*(CWFLOAT)winWidth, m_fViewPortHeight*(CWFLOAT)winHeight,
		m_fViewPortMinDepth, m_fViewPortMaxDepth);
	CW_SAFE_RETAIN(m_pViewPort);
}

CWVOID cwRenderer::resize()
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	if (m_pViewPort) {
		m_pViewPort->refresh(
			m_fViewPortTopLeftX*(CWFLOAT)winWidth, m_fViewPortTopLeftY*(CWFLOAT)winHeight,
			m_fViewPortWidth*(CWFLOAT)winWidth, m_fViewPortHeight*(CWFLOAT)winHeight,
			m_fViewPortMinDepth, m_fViewPortMaxDepth);
	}
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
	m_pCurrCamera = nullptr;
	m_pCurrShader = nullptr;
	m_pCurrRenderStage = nullptr;

	m_pRenderListHead = buildStageList();
}

CWVOID cwRenderer::render()
{
	cwRepertory::getInstance().getDevice()->setViewPort(m_pViewPort);

	if (m_pRenderListHead) {
		sRendererListNode* pElement = nullptr;

		DL_FOREACH(m_pRenderListHead, pElement) {
			m_pCurrRenderStage = pElement->m_pStage;
			if (m_pCurrRenderStage->getEnable())
				this->render(m_pCurrRenderStage);
		}
	}
}

CWVOID cwRenderer::end()
{
	cwRepertory::getInstance().getDevice()->swap();
}

CWVOID cwRenderer::render(cwStage* pStage)
{
	if (!pStage) return;

	pStage->begin();
	pStage->render();
	pStage->end();
}

CWVOID cwRenderer::render(cwRenderBatch* pBatch)
{
	if (pBatch && pBatch->m_pEffect) {
		cwDevice* pDevice = cwRepertory::getInstance().getDevice();

		this->setCurrShader(pBatch->m_pEffect->getShader());
		pDevice->setShaderWorldTrans(pBatch->m_pEffect->getShader(), pBatch->m_nWorldTrans, m_pCurrCamera);

		pBatch->m_pEffect->render(pBatch);
	}
}

cwStage* cwRenderer::getStage(const CWSTRING& strName)
{
	for (auto pStage : m_nVecStage) {
		if (pStage->getName() == strName) return pStage;
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
		sRendererListNode* pNode = getAvaiableListNode();
		if (pNode) {
			pNode->m_pStage = pStage;
			DL_APPEND(pHead, pNode);
		}
	}

	return pHead;
}

CWVOID cwRenderer::configLight()
{
	if (!m_pCurrShader) return;
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene) return;

	configDirectionalLight();
	configPointLight();
	configSpotLight();
}

CWVOID cwRenderer::configDirectionalLight()
{
	if (!m_pCurrShader->hasVariable(eShaderParamDirectionalLight) || 
		!m_pCurrShader->hasVariable(eShaderParamDirectionalLightCnt)) return;

	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwVector<cwDirectionalLight*>& vecLight = pCurrScene->getDirectionalLights();
	if (vecLight.empty()) {
		m_pCurrShader->setVariableInt(eShaderParamDirectionalLightCnt, 0);
		return;
	}

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamDirectionalLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamDirectionalLightCnt, (CWINT)(vecLight.size()));
}

CWVOID cwRenderer::configPointLight()
{
	if (!m_pCurrShader->hasVariable(eShaderParamPointLight) ||
		!m_pCurrShader->hasVariable(eShaderParamPointLightCnt)) return;

	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwVector<cwPointLight*>& vecLight = pCurrScene->getPointLights();
	if (vecLight.empty()) {
		m_pCurrShader->setVariableInt(eShaderParamPointLightCnt, 0);
		return;
	}

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamPointLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamPointLightCnt, (CWINT)(vecLight.size()));
}

CWVOID cwRenderer::configSpotLight()
{
	if (!m_pCurrShader->hasVariable(eShaderParamSpotLight) ||
		!m_pCurrShader->hasVariable(eShaderParamSpotLightCnt)) return;

	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwVector<cwSpotLight*>& vecLight = pCurrScene->getSpotLights();
	if (vecLight.empty()) {
		m_pCurrShader->setVariableInt(eShaderParamSpotLightCnt, 0);
		return;
	}

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it, ++index) {
		m_pCurrShader->setVariableData(eShaderParamSpotLight, index, (*it)->data(), 0, (*it)->size());
	}

	m_pCurrShader->setVariableInt(eShaderParamSpotLightCnt, (CWINT)(vecLight.size()));
}

cwRay cwRenderer::getPickingRayWorld(CWFLOAT fPosX, CWFLOAT fPosY)
{
	cwRay ray;

	const cwMatrix4X4& matProj = m_pRendererCamera->getProjMatrix();
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	CWFLOAT fViewPortLeftX = fPosX - (CWFLOAT)winWidth*m_fViewPortTopLeftX;
	CWFLOAT fViewPortLeftY = fPosY - (CWFLOAT)winHeight*m_fViewPortTopLeftY;

	CWFLOAT fViewWidth  = (CWFLOAT)winWidth*m_fViewPortWidth;
	CWFLOAT fViewHeight = (CWFLOAT)winHeight*m_fViewPortHeight;

	CWFLOAT vx = (+2.0f * fViewPortLeftX / fViewWidth - 1.0f) / matProj.m11;
	CWFLOAT vy = (-2.0f * fViewPortLeftY / fViewHeight + 1.0f) / matProj.m22;

	const cwMatrix4X4& matView = m_pRendererCamera->getViewMatrix();
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
