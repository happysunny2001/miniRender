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
m_pDefaultCamera(nullptr)
{

}

cwEngine::~cwEngine()
{
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	CW_SAFE_RELEASE_NULL(m_pRenderer);
	m_pDefaultCamera = nullptr;
}

CWBOOL cwEngine::init()
{
	buildDefaultCamera();
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
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	m_pCurrScene = pScene;
}

CWVOID cwEngine::mainLoop(CWFLOAT dt)
{
	cwRepertory& repertory = cwRepertory::getInstance();

	repertory.getEventManager()->dispatchEvent();
	repertory.getSchedulerManager()->update(dt);
	render();
	repertory.getAutoReleasePool()->clear();
}

CWVOID cwEngine::buildDefaultCamera()
{
	cwCamera* pDefCamera = cwCamera::create();
	pDefCamera->setName("default");
	m_nMapCameras.insert(pDefCamera->getName(), pDefCamera);

	cwHomogeneousCamera* pHomoCamera = cwHomogeneousCamera::create();
	pHomoCamera->setName("Homo");
	m_nMapCameras.insert(pHomoCamera->getName(), pHomoCamera);

	m_pDefaultCamera = m_nMapCameras.find("default")->second;
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
	if (!pCamera) return false;

	m_nMapCameras.erase(pCamera->getName());
	return true;
}

CWVOID cwEngine::addCamera(cwCamera* pCamera)
{
	if (pCamera) {
		m_nMapCameras.insert(pCamera->getName(), pCamera);
	}
}

CWVOID cwEngine::render()
{
	if (m_pRenderer) {
		m_pRenderer->begin();
		m_pRenderer->render();
		m_pRenderer->end();
	}
}

NS_MINIR_END
