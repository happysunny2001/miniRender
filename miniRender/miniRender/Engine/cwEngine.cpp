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
m_uNodeVectorCounter(0)
{

}

cwEngine::~cwEngine()
{
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	CW_SAFE_RELEASE_NULL(m_pRenderer);
	CW_SAFE_RELEASE_NULL(m_pSpatial);
	m_pDefaultCamera = nullptr;
}

CWBOOL cwEngine::init()
{
	buildDefaultCamera();

	m_pSpatial = cwSpatialFactory::createSpatial("Octree");
	CW_SAFE_RETAIN(m_pSpatial);

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

	if (m_pSpatial) {
		m_pSpatial->update();
	}

	render();
	repertory.getAutoReleasePool()->clear();
}

CWVOID cwEngine::buildDefaultCamera()
{
	cwRepertory& repertory = cwRepertory::getInstance();
	CWFLOAT fScreenWidth  = static_cast<CWFLOAT>(repertory.getUInt(gValueWinWidth));
	CWFLOAT fScreenHeight = static_cast<CWFLOAT>(repertory.getUInt(gValueWinHeight));

	cwCamera* pDefCamera = cwCamera::create();
	pDefCamera->setName("Default");
	pDefCamera->updateProjMatrix(0.25f*cwMathUtil::cwPI, fScreenWidth / fScreenHeight, 1.0f, 1000.0f);
	this->addCamera(pDefCamera);

	cwHomogeneousCamera* pHomoCamera = cwHomogeneousCamera::create();
	pHomoCamera->setName("Homo");
	this->addCamera(pHomoCamera);

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

cwVector<cwRenderNode*>* cwEngine::getVisibleNodes(cwCamera* pCamera, eSceneObjectType eType)
{
	if (pCamera && m_pSpatial) {
		auto it = m_nMapVisibleNodes.find(pCamera);
		if (it != m_nMapVisibleNodes.end()) return (it->second);

		cwVector<cwRenderNode*>& vecNodes = m_nVecVisiableNodes[m_uNodeVectorCounter++];
		m_pSpatial->intersection(pCamera->getFrustum(), vecNodes, eType, CWTRUE);

		m_nMapVisibleNodes[pCamera] = &vecNodes;

		return &vecNodes;
	}

	return nullptr;
}

CWVOID cwEngine::clearVisibleNodes()
{
	m_nMapVisibleNodes.clear();
	for (auto it = m_nVecVisiableNodes.begin(); it != m_nVecVisiableNodes.end(); ++it) {
		it->clear();
	}

	m_uNodeVectorCounter = 0;
}

NS_MINIR_END
