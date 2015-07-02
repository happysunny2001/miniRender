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
#include "Camera/cwCamera.h"
#include "Device/cwDevice.h"
#include "Ref/cwAutoReleasePool.h"
#include "Event/cwEventManager.h"

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
m_pCurrCamera(nullptr)
{

}

cwEngine::~cwEngine()
{
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	m_pCurrCamera = nullptr;
}

bool cwEngine::init()
{
	buildDefaultCamera();
	return true;
}

void cwEngine::setScene(cwScene* pScene)
{
	if (pScene == m_pCurrScene) return;
	CW_SAFE_RETAIN(pScene);
	CW_SAFE_RELEASE_NULL(m_pCurrScene);
	m_pCurrScene = pScene;
}

void cwEngine::mainLoop(CWFLOAT dt)
{
	cwRepertory& repertory = cwRepertory::getInstance();

	repertory.getEventManager()->dispatchEvent();
	render();
	repertory.getAutoReleasePool()->clear();
}

void cwEngine::buildDefaultCamera()
{
	m_nVecCameras.pushBack(cwCamera::create());
}

cwCamera* cwEngine::getDefaultCamera()
{
	if (m_nVecCameras.empty()) return nullptr;
	return m_nVecCameras.at(0);
}

bool cwEngine::removeCamera(cwCamera* pCamera)
{
	if (!pCamera) return false;
	//last camera can't remove
	if (m_nVecCameras.size() == 1) return false;
	m_nVecCameras.erase(pCamera, true);
	return true;
}

cwCamera* cwEngine::getCurrentCamera()
{
	return m_pCurrCamera;
}

void cwEngine::render()
{
	m_pCurrCamera = getDefaultCamera();
//	cwRepertory::getInstance().setCurrentCamera(getDefaultCamera());
	cwRepertory::getInstance().getDevice()->beginDraw();

	if (m_pCurrScene) {
		m_pCurrScene->render();
	}

	cwRepertory::getInstance().getDevice()->endDraw();

	m_pCurrCamera = nullptr;
}

NS_MINIR_END
