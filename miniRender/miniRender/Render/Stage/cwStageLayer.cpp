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

#include "cwStageLayer.h"
#include "Camera/cwCamera.h"
#include "Texture/cwTexture.h"
#include "effect/cwEffect.h"
#include "Entity/cwRenderNode.h"
#include "Entity/cwScene.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Device/cwDevice.h"
#include "Shader/cwShaderConstant.h"
#include "ViewPort/cwViewPort.h"
#include "Light/cwDirectionalLight.h"
#include "Light/cwPointLight.h"
#include "Light/cwSpotLight.h"
#include "Sprite/cwRenderNode2D.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "cwStage.h"

NS_MINIR_BEGIN

cwStageLayer* cwStageLayer::create()
{
	cwStageLayer* pStageLayer = new cwStageLayer();
	if (pStageLayer && pStageLayer->init()) {
		pStageLayer->autorelease();
		return pStageLayer;
	}

	CW_SAFE_DELETE(pStageLayer);
	return nullptr;
}

cwStageLayer::cwStageLayer() : 
m_nFilterType(eStageLayerFliterNone),
m_pCamera(nullptr),
m_pRenderTarget(nullptr),
m_pDepthStencil(nullptr),
m_iCommandBatchIndex(0),
m_pUniformEffect(nullptr),
m_pLastUsingBatch(nullptr),
m_pViewPort(nullptr),
m_bIsRefreshRenderTarget(CWFALSE),
m_pStencil(nullptr),
m_pBlend(nullptr)
{

}

cwStageLayer::~cwStageLayer()
{
	CW_SAFE_RELEASE_NULL(m_pCamera);
	CW_SAFE_RELEASE_NULL(m_pRenderTarget);
	CW_SAFE_RELEASE_NULL(m_pDepthStencil);
	CW_SAFE_RELEASE_NULL(m_pUniformEffect);
	CW_SAFE_RELEASE_NULL(m_pViewPort);
	CW_SAFE_RELEASE_NULL(m_pStencil);
	CW_SAFE_RELEASE_NULL(m_pBlend);
	m_pLastUsingBatch = nullptr;
}

CWBOOL cwStageLayer::init()
{
	this->buildViewPort();
	this->buildBlend();
	this->buildStencil();

	return CWTRUE;
}

CWBOOL cwStageLayer::buildViewPort()
{
	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	m_pViewPort = cwRepertory::getInstance().getDevice()->createViewPort(0, 0, (CWFLOAT)winWidth, (CWFLOAT)winHeight);
	CW_SAFE_RETAIN(m_pViewPort);

	return CWTRUE;
}

CWBOOL cwStageLayer::buildViewPort(CWFLOAT fLeft, CWFLOAT fTop, CWFLOAT fWidth, CWFLOAT fHeight, CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	if (m_pViewPort) {
		m_pViewPort->refresh(fLeft, fTop, fWidth, fHeight, fMinDepth, fMaxDepth);
	}
	else {
		m_pViewPort = cwRepertory::getInstance().getDevice()->createViewPort(fLeft, fTop, fWidth, fHeight, fMinDepth, fMaxDepth);
		CW_SAFE_RETAIN(m_pViewPort);
	}

	return CWTRUE;
}

CWBOOL cwStageLayer::buildStencil()
{
	return CWTRUE;
}

CWBOOL cwStageLayer::buildBlend()
{
	return CWTRUE;
}

CWVOID cwStageLayer::setCamera(cwCamera* pCamera)
{
	CW_SAFE_RETAIN(pCamera);
	CW_SAFE_RELEASE(m_pCamera);
	m_pCamera = pCamera;
}

CWVOID cwStageLayer::setRenderTarget(cwTexture* pTexture)
{
	CW_SAFE_RETAIN(pTexture);
	CW_SAFE_RELEASE(m_pRenderTarget);
	m_pRenderTarget = pTexture;
}

CWVOID cwStageLayer::setDepthStencil(cwTexture* pTexture)
{
	CW_SAFE_RETAIN(pTexture);
	CW_SAFE_RELEASE(m_pDepthStencil);
	m_pDepthStencil = pTexture;
}

CWVOID cwStageLayer::addSelfRenderNode(cwRenderNode* pRenderNode)
{
	m_nVecSelfNode.pushBack(pRenderNode);
}

CWVOID cwStageLayer::removeSelfRenderNode(cwRenderNode* pRenderNode)
{
	m_nVecSelfNode.erase(pRenderNode);
}

CWVOID cwStageLayer::addOuterStage(cwStage* pStage)
{
	if (pStage) {
		m_nVecOuterStage.pushBack(pStage);
	}
}

CWVOID cwStageLayer::setUniformEffect(cwEffect* pEffect)
{
	CW_SAFE_RETAIN(pEffect);
	CW_SAFE_RELEASE(m_pUniformEffect);
	m_pUniformEffect = pEffect;
}

CWVOID cwStageLayer::reset()
{
	m_iCommandBatchIndex = 0;
	m_pLastUsingBatch = nullptr;
	m_nMapCommandBatch.clear();
}

CWVOID cwStageLayer::bindingRenderTarget()
{
	cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	if (m_bIsRefreshRenderTarget) {
		pDevice->setRenderTarget(m_pRenderTarget);
		pDevice->setDepthStencil(m_pDepthStencil);
		pDevice->beginDraw(m_bIsClearColor, m_bIsClearDepth, m_bIsClearStencil);
	}
}

CWVOID cwStageLayer::begin()
{
	this->reset();
	this->batchRenderNodes();

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	pDevice->setViewPort(m_pViewPort);

	pDevice->setStencil(m_pStencil);
	pDevice->setBlend(m_pBlend);

	this->bindingRenderTarget();
}

CWVOID cwStageLayer::render()
{
	for (auto it = m_nMapCommandBatch.begin(); it != m_nMapCommandBatch.end(); ++it) {
		cwShader* pShader = it->first;
		if (pShader) {
			this->preFrameConfig(pShader);
			this->configOuterStageParam(pShader);

			cwScene* pScene = cwRepertory::getInstance().getEngine()->getCurrScene();
			if (pScene) {
				this->configLight(pScene, pShader);
			}

			for (auto itBatch = it->second.begin(); itBatch != it->second.end(); ++itBatch) {
				(*itBatch)->render();
			}
		}
	}
}

CWVOID cwStageLayer::end()
{
	cwRepertory::getInstance().getDevice()->endDraw();
	cwRepertory::getInstance().getDevice()->clearShaderResource();
}

CWVOID cwStageLayer::preFrameConfig(cwShader* pShader)
{
	if (pShader && m_pCamera) {
		pShader->setVariableMatrix(CW_SHADER_MAT_VIEWPROJ, m_pCamera->getViewProjMatrix());
		pShader->setVariableMatrix(CW_SHADER_MAT_VIEW, m_pCamera->getViewMatrix());
		pShader->setVariableMatrix(CW_SHADER_MAT_PROJ, m_pCamera->getProjMatrix());

		const cwVector3D& pos = m_pCamera->getPos();
		pShader->setVariableData(CW_SHADER_POS_EYE_WORLD, (CWVOID*)&pos, 0, sizeof(cwVector3D));

		cwVector4D nearFar = cwVector4D::ZERO;
		nearFar.x = m_pCamera->getFarZ();
		nearFar.y = m_pCamera->getNearZ();
		pShader->setVariableData(CW_SHADER_CAMERA_NEAR_FAR_Z, (CWVOID*)&nearFar, 0, sizeof(cwVector4D));
	}
}

CWVOID cwStageLayer::configOuterStageParam(cwShader* pShader)
{
	if (m_nVecOuterStage.size() > 0) {
		for (auto pStage : m_nVecOuterStage) {
			if (pStage) {
				pStage->bindingResultParameter(pShader);
			}
		}
	}
}

CWVOID cwStageLayer::configLight(cwScene* pScene, cwShader* pShader)
{
	this->configDirectionalLight(pScene, pShader);
	this->configPointLight(pScene, pShader);
	this->configSpotLight(pScene, pShader);
}

CWVOID cwStageLayer::configDirectionalLight(cwScene* pScene, cwShader* pShader)
{

}

CWVOID cwStageLayer::configPointLight(cwScene* pScene, cwShader* pShader)
{

}

CWVOID cwStageLayer::configSpotLight(cwScene* pScene, cwShader* pShader)
{

}

CWVOID cwStageLayer::batchRenderNodes()
{
	if (m_nFilterType & eStageLayerFliterSelf)
		this->batchSelfRenderNodes();

	if (m_nFilterType & eStageLayerFliterScene)
		this->batchSceneRenderNodes();

	if (m_nFilterType & eStageLayerFliter2D)
		this->batchScene2DNodes();
}

CWVOID cwStageLayer::batchSelfRenderNodes()
{
	for (auto it = m_nVecSelfNode.begin(); it != m_nVecSelfNode.end(); ++it) {
		this->addRenderNode(*it);
	}
}

CWVOID cwStageLayer::batchSceneRenderNodes()
{
	if (!m_pCamera) return;

	m_nVecSceneRenderNodes.clear();
	cwRepertory::getInstance().getEngine()->getVisibleNodes(m_pCamera, eRenderTypeEntity, m_nVecSceneRenderNodes);

	for (auto it = m_nVecSceneRenderNodes.begin(); it != m_nVecSceneRenderNodes.end(); ++it) {
		this->addRenderNode(*it);
	}
}

CWVOID cwStageLayer::batchScene2DNodes()
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene) return;

	std::vector<cwRenderNode2D*>& vecNodes2D = pCurrScene->getRenderNodes2D();
	for (auto it = vecNodes2D.begin(); it != vecNodes2D.end(); ++it) {
		this->addRenderNode((*it));
	}
}

CWBOOL cwStageLayer::addRenderNode(cwRenderNode* pNode)
{
	return addRenderNodeWithNodesEffect(pNode) ? CWTRUE : addRenderNodeWithUniformEffect(pNode);

	//cwEffect* pEffect = pNode->getEffect();
	//if (pEffect && pEffect->getShader()) {
	//	cwCommandBatch* pCmdBatch = getCommandBatch(pEffect->getShader());
	//	if (pCmdBatch) {
	//		pCmdBatch->addRenderNode(pNode);
	//		m_pLastUsingBatch = pCmdBatch;
	//		return CWTRUE;
	//	}
	//}

	//if (m_pUniformEffect && m_pUniformEffect->getShader()) {
	//	cwCommandBatch* pCmdBatch = getCommandBatch(m_pUniformEffect->getShader());
	//	if (pCmdBatch) {
	//		pCmdBatch->addRenderNode(pNode, m_pUniformEffect);
	//		m_pLastUsingBatch = pCmdBatch;
	//		return CWTRUE;
	//	}
	//}

	//return CWFALSE;
}

CWBOOL cwStageLayer::addRenderNodeWithNodesEffect(cwRenderNode* pNode)
{
	cwEffect* pEffect = pNode->getEffect();
	if (pEffect && pEffect->getShader()) {
		cwCommandBatch* pCmdBatch = getCommandBatch(pEffect->getShader());
		if (pCmdBatch) {
			pCmdBatch->addRenderNode(pNode);
			m_pLastUsingBatch = pCmdBatch;
			return CWTRUE;
		}
	}

	return CWFALSE;
}

CWBOOL cwStageLayer::addRenderNodeWithUniformEffect(cwRenderNode* pNode)
{
	if (m_pUniformEffect && m_pUniformEffect->getShader()) {
		cwCommandBatch* pCmdBatch = getCommandBatch(m_pUniformEffect->getShader());
		if (pCmdBatch) {
			pCmdBatch->addRenderNode(pNode, m_pUniformEffect);
			m_pLastUsingBatch = pCmdBatch;
			return CWTRUE;
		}
	}

	return CWFALSE;
}

cwCommandBatch* cwStageLayer::getFreeCommandBatch()
{
	if (m_iCommandBatchIndex < CW_COMMAND_BATCH_MAX) {
		cwCommandBatch& commandBatch = m_nCommandBatch[m_iCommandBatchIndex++];
		commandBatch.reset();
		return &commandBatch;
	}

	return nullptr;
}

cwCommandBatch* cwStageLayer::getCommandBatch(cwShader* pKeyShader)
{
	if (m_pLastUsingBatch && 
		m_pLastUsingBatch->getUsingShader() == pKeyShader &&
		!m_pLastUsingBatch->full()) return m_pLastUsingBatch;

	auto it = m_nMapCommandBatch.find(pKeyShader);
	if (it != m_nMapCommandBatch.end()) {
		if (!it->second.empty()) {
			auto lastBatch = it->second.back();
			if (!lastBatch->full()) return lastBatch;
		}

		cwCommandBatch* pFreeBatch = getFreeCommandBatch();
		if (!pFreeBatch) return nullptr;
		pFreeBatch->setUsingShader(pKeyShader);
		it->second.push_back(pFreeBatch);
		return pFreeBatch;
	}
	else {
		cwCommandBatch* pFreeBatch = getFreeCommandBatch();
		if (!pFreeBatch) return nullptr;
		pFreeBatch->setUsingShader(pKeyShader);

		std::vector<cwCommandBatch*> vec;
		vec.push_back(pFreeBatch);
		m_nMapCommandBatch[pKeyShader] = vec;
		return pFreeBatch;
	}
}

NS_MINIR_END
