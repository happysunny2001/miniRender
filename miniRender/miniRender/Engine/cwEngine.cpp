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

#include "cwEngine.h"
#include "Entity/cwScene.h"
#include "Entity/cwEntity.h"
#include "Material/cwMaterial.h"
#include "Camera/cwCamera.h"
#include "Camera/cwHomogeneousCamera.h"
#include "Camera/cwOrthoCamera.h"
#include "Device/cwDevice.h"
#include "Ref/cwAutoReleasePool.h"
#include "Event/cwEventManager.h"
#include "Scheduler/cwSchedulerManager.h"
#include "Shader/cwShader.h"
#include "Effect/cwEffect.h"
#include "Render/cwRenderer.h"
#include "Parser/cwParserManager.h"
#include "Parser/cwRendererParser.h"
#include "Platform/cwFileSystem.h"
#include "Repertory/cwRepertory.h"
#include "SpatialSorting/cwSpatial.h"
#include "SpatialSorting/cwSpatialFactory.h"
#include "Sprite/cwRenderNode2D.h"
#include "Sprite/cwSpriteManager.h"
#include "Sprite/cwLabel.h"
#include "Event/cwTouchEvent.h"
#include "Resource/cwResourceLoader.h"

#include <sstream>

NS_MINIR_BEGIN

cwEngine* cwEngine::create()
{
	cwEngine* pEngine = new cwEngine();
	if (pEngine && pEngine->init()) {
		pEngine->autorelease();
		return pEngine;
	}

	CW_SAFE_DELETE(pEngine);
	return nullptr;
}

cwEngine::cwEngine():
m_pCurrScene(nullptr),
m_pRenderer(nullptr),
m_pDefaultCamera(nullptr),
m_pSpatial(nullptr),
m_uNodeVectorCounter(0),
m_pSpriteManager(nullptr),
m_fFrameCounter(0),
m_uFrameRate(0),
m_uFrameCounter(0),
m_bShowFrame(CWFALSE),
m_pLabelFrameRate(nullptr)
{

}

cwEngine::~cwEngine()
{
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	CW_SAFE_RELEASE_NULL(m_pRenderer);
	CW_SAFE_RELEASE_NULL(m_pSpatial);
	CW_SAFE_RELEASE_NULL(m_pSpriteManager);
	CW_SAFE_RELEASE_NULL(m_pLabelFrameRate);
	m_pDefaultCamera = nullptr;
}

CWBOOL cwEngine::init()
{
	buildDefaultCamera();
	
	m_pSpatial = cwRepertory::getInstance().getSpatialFactory()->createSpatial("LooseOctree");
	CW_SAFE_RETAIN(m_pSpatial);

	buildSpriteManager();
	buildFrameRateLabel();

	return true;
}

CWVOID cwEngine::loadRenderer(const CWSTRING& strConfFile)
{
	CW_SAFE_RELEASE_NULL(m_pRenderer);

	cwRendererParser* pRendererParser = static_cast<cwRendererParser*>(cwRepertory::getInstance().getParserManager()->getParser(eParserRenderer));
	if (pRendererParser) {
		CWSTRING strFilePath = cwRepertory::getInstance().getFileSystem()->getFullFilePath(strConfFile);
		m_pRenderer = pRendererParser->parse(strFilePath);
		CW_SAFE_RETAIN(m_pRenderer);

		pRendererParser->deferParse(m_pRenderer);
	}
}

CWVOID cwEngine::buildSpriteManager()
{
	m_pSpriteManager = cwSpriteManager::create();
	CW_SAFE_RETAIN(m_pSpriteManager);
}

CWVOID cwEngine::buildFrameRateLabel()
{
	m_pLabelFrameRate = cwLabel::create("0", "frame_text.png", '0', 10);
	CW_SAFE_RETAIN(m_pLabelFrameRate);

	CWINT m_uScreenWidth  = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWINT m_uScreenHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	m_pLabelFrameRate->setPosition(CWFLOAT(-(m_uScreenWidth >> 1)) + 30.0f, CWFLOAT(-(m_uScreenHeight >> 1)) + 10.0f);
	//m_pLabelFrameRate->setPosition(cwVector3D::ZERO);
	m_pLabelFrameRate->transform();
	m_pLabelFrameRate->refreshTransform();
	m_pLabelFrameRate->refreshBoundingBox();
}

CWVOID cwEngine::setScene(cwScene* pScene)
{
	if (pScene == m_pCurrScene) return;
	CW_SAFE_RETAIN(pScene);

	if (m_pSpatial) {
		m_pSpatial->remove(m_pCurrScene);
	}

	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	m_pCurrScene = pScene;
}

CWVOID cwEngine::mainLoop(CWFLOAT dt)
{
	cwRepertory& repertory = cwRepertory::getInstance();

	repertory.getEventManager()->dispatchEvent();
	repertory.getSchedulerManager()->update(dt);
	repertory.getResourceLoader()->update(dt);

	if (m_pSpatial) {
		m_pSpatial->update();
	}

	render();
	repertory.getAutoReleasePool()->clear();

	refreshFrameRate(dt);
}

CWVOID cwEngine::refreshFrameRate(CWFLOAT dt)
{
	if (m_bShowFrame) {
		m_uFrameCounter++;
		m_fFrameCounter += dt;
		if (m_fFrameCounter >= 1.0f) {
			m_fFrameCounter = 0;
			m_uFrameRate = m_uFrameCounter;
			m_uFrameCounter = 0;

			stringstream ss;
			ss << m_uFrameRate;
			if (m_pLabelFrameRate) {
				m_pLabelFrameRate->setString(ss.str());
			}
		}
	}
}

CWVOID cwEngine::buildDefaultCamera()
{
	cwRepertory& repertory = cwRepertory::getInstance();
	CWFLOAT fScreenWidth  = static_cast<CWFLOAT>(repertory.getUInt(gValueWinWidth));
	CWFLOAT fScreenHeight = static_cast<CWFLOAT>(repertory.getUInt(gValueWinHeight));

	cwCamera* pDefCamera = cwCamera::create();
	pDefCamera->setName("Default");
	pDefCamera->updateProjMatrixFov(0.25f*cwMathUtil::cwPI, fScreenWidth / fScreenHeight, 1.0f, 1000.0f);
	this->addCamera(pDefCamera);

	cwHomogeneousCamera* pHomoCamera = cwHomogeneousCamera::create();
	pHomoCamera->setName("Homo");
	this->addCamera(pHomoCamera);

	cwOrthoCamera* pOrthoCamera = cwOrthoCamera::create();
	pOrthoCamera->setName("Ortho");
	pOrthoCamera->updateProjMatrix(fScreenWidth, fScreenHeight, 1.0f, 1000.0f);
	this->addCamera(pOrthoCamera);

	m_pDefaultCamera = getCamera("Default"); 
}

cwCamera* cwEngine::getDefaultCamera()
{
	return m_pDefaultCamera;
}

cwCamera* cwEngine::getCamera(const CWSTRING& strName)
{
	if (m_nMapCameras.find(strName) != m_nMapCameras.end()) {
		return m_nMapCameras.find(strName)->second;
	}

	return nullptr;
}

CWBOOL cwEngine::removeCamera(cwCamera* pCamera)
{
	if (!pCamera) return CWFALSE;

	if (m_pDefaultCamera == pCamera) {
		m_pDefaultCamera = nullptr;
	}

	m_nMapCameras.erase(pCamera->getName());
	m_nVecVisiableNodes.pop_back();
	return CWTRUE;
}

CWVOID cwEngine::addCamera(cwCamera* pCamera)
{
	if (pCamera) {
		m_nMapCameras.insert(pCamera->getName(), pCamera);

		cwVector<cwRenderNode*> vecNodes;
		vecNodes.reserve(100);
		m_nVecVisiableNodes.push_back(vecNodes);
	}
}

CWVOID cwEngine::render()
{
	if (m_pRenderer) {
		m_pRenderer->begin();
		m_pRenderer->render();
		m_pRenderer->end();
	}

	clearVisibleNodes();
}

CWVOID cwEngine::renderSprite()
{
	if (m_pSpriteManager) {
		m_pSpriteManager->begin();
		m_pSpriteManager->render();

		if (m_bShowFrame && m_pLabelFrameRate) {
			m_pSpriteManager->renderBatch(m_pLabelFrameRate);
		}

		m_pSpriteManager->end();
	}
}

CWVOID cwEngine::addNode2D(cwRenderNode2D* pNode2D)
{
	if (m_pSpriteManager)
		m_pSpriteManager->addNode(pNode2D);
}

CWVOID cwEngine::removeNode2D(cwRenderNode2D* pNode2D)
{
	if (m_pSpriteManager)
		m_pSpriteManager->removeNode(pNode2D);
}

CWBOOL cwEngine::insertSpatialNode(cwRenderNode* pNode)
{
	if (m_pSpatial)
		return m_pSpatial->insert(pNode);
	return CWFALSE;
}

CWBOOL cwEngine::removeSpatialNode(cwRenderNode* pNode)
{
	if (m_pSpatial)
		return m_pSpatial->remove(pNode);
	return CWFALSE;
}

CWVOID cwEngine::refreshSpatialNode(cwRenderNode* pNode)
{
	if (m_pSpatial)
		m_pSpatial->refresh(pNode);
}

CWVOID cwEngine::extractRenderNodes(sVisibleNodesResult& result, std::vector<cwRenderNode*>& vecNodes, eRenderType eType)
{
	for (auto it = result.m_pVecVisibleNodes->begin(); it != result.m_pVecVisibleNodes->end(); ++it) {
		if ((*it)->getRenderType() == eType)
			vecNodes.push_back(*it);
	}
}

CWVOID cwEngine::getVisibleNodes(cwCamera* pCamera, eRenderType eType, std::vector<cwRenderNode*>& vecNodes)
{
	if (pCamera && m_pSpatial) {
		sVisibleNodesResult* pCacheResult = nullptr;
		for (auto it = m_nVisibleResult.begin(); it != m_nVisibleResult.end(); ++it) {
			if (it->m_pCamera == pCamera) {
				pCacheResult = &(*it);
				break;
			}
		}

		if (pCacheResult) {
			extractRenderNodes(*pCacheResult, vecNodes, eType);
			return;
		}

		cwVector<cwRenderNode*>* pVisibleNode = getEmptyNodeList();
		m_pSpatial->intersection(pCamera->getFrustum(), *pVisibleNode, CWTRUE);

		sVisibleNodesResult result;
		result.m_pCamera = pCamera;
		result.m_pVecVisibleNodes = pVisibleNode;
		m_nVisibleResult.push_back(result);

		extractRenderNodes(result, vecNodes, eType);
	}
}

cwRenderNode* cwEngine::getScreenClickNode(cwTouch* pTouch)
{
	cwRay ray = m_pRenderer->getPickingRayWorld(pTouch);

	cwVector<cwRenderNode*> vecRet;
	m_pSpatial->intersection(ray, vecRet, CWTRUE);
	if (vecRet.empty()) return nullptr;

	const cwMatrix4X4& matView = m_pRenderer->getRendererCamera()->getViewMatrix();

	cwRenderNode* pNearestNode = nullptr;
	CWFLOAT fMinDist = cwMathUtil::cwInfinity;
	for (auto pNode : vecRet) {
		if (pNode->getRenderType() == eRenderTypeEntity) {
			cwMatrix4X4 matTrans = pNode->getTransformMatrix()*matView;
			if (matTrans.m43 < fMinDist) {
				fMinDist = matTrans.m43;
				pNearestNode = pNode;
			}
		}
	}

	return pNearestNode;
}

cwVector<cwRenderNode*>* cwEngine::getEmptyNodeList()
{
	if (m_uNodeVectorCounter >= m_nVecVisiableNodes.size()) {
		cwVector<cwRenderNode*> vecNodes;
		vecNodes.reserve(100);
		m_nVecVisiableNodes.push_back(vecNodes);
	}

	return &(m_nVecVisiableNodes[m_uNodeVectorCounter++]);
}

CWVOID cwEngine::clearVisibleNodes()
{
	m_nVisibleResult.clear();
	for (auto it = m_nVecVisiableNodes.begin(); it != m_nVecVisiableNodes.end(); ++it) {
		it->clear();
	}

	m_uNodeVectorCounter = 0;
}

CWVOID cwEngine::resize()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	CWFLOAT fov = repertory.getFloat(gValueFov);
	CWFLOAT nearZ = repertory.getFloat(gValueNearZ);
	CWFLOAT farZ = repertory.getFloat(gValueFarZ);
	CWINT width = repertory.getUInt(gValueWinWidth);
	CWINT height = repertory.getUInt(gValueWinHeight);
	CWFLOAT aspect = (CWFLOAT)width / (CWFLOAT)height;

	for (auto it = m_nMapCameras.begin(); it != m_nMapCameras.end(); ++it) {
		it->second->updateProjMatrixFov(fov, aspect, nearZ, farZ);
	}

	cwCamera* pOrthoCamera = this->getCamera("Ortho");
	if (pOrthoCamera) {
		pOrthoCamera->updateProjMatrix((CWFLOAT)width, (CWFLOAT)height, nearZ, farZ);
	}

	m_pLabelFrameRate->setPosition(CWFLOAT(-(width >> 1)) + 30.0f, CWFLOAT(-(height >> 1)) + 10.0f);
	m_pLabelFrameRate->transform();
	m_pLabelFrameRate->refreshTransform();
	m_pLabelFrameRate->refreshBoundingBox();

	if (m_pRenderer) {
		m_pRenderer->resize();
	}
}

NS_MINIR_END
