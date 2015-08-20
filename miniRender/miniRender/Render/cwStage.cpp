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

#include "cwStage.h"
#include "Camera/cwCamera.h"
#include "Effect/cwEffect.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Entity/cwScene.h"
#include "Entity/cwEntity.h"
#include "Device/cwDevice.h"
#include "cwStageLayer.h"
#include "cwRenderer.h"
#include "Generator/cwRenderGenerator.h"

NS_MINIR_BEGIN

cwStage* cwStage::create()
{
	cwStage* pStage = new cwStage();
	if (pStage) {
		pStage->autorelease();
		return pStage;
	}

	return nullptr;
}

cwStage::cwStage() :
m_pCamera(nullptr),
m_bEnable(CWTRUE),
m_eType(eStageTypeNormal),
m_pViewPort(nullptr),
m_pRenderTarget(nullptr),
m_pStageEffect(nullptr),
m_bClearColor(CWTRUE),
m_bClearDepth(CWTRUE),
m_bClearStencil(CWTRUE),
m_bRefreshRenderTarget(CWTRUE)
{

}

cwStage::~cwStage()
{
	CW_SAFE_RELEASE_NULL(m_pCamera);
	CW_SAFE_RELEASE_NULL(m_pViewPort);
	CW_SAFE_RELEASE_NULL(m_pRenderTarget);
	CW_SAFE_RELEASE_NULL(m_pStageEffect);

	clearStageEntity();
	clearStageLayer(CWTRUE);
	clearStageGenerator();
}

CWVOID cwStage::setViewPort(cwViewPort* pView)
{
	if (m_pViewPort == pView) return;

	CW_SAFE_RETAIN(pView);
	CW_SAFE_RELEASE_NULL(m_pViewPort);
	m_pViewPort = pView;
}

CWVOID cwStage::setRenderTexture(cwRenderTexture* pRenderTexture)
{
	if (pRenderTexture == m_pRenderTarget) return;

	CW_SAFE_RETAIN(pRenderTexture);
	CW_SAFE_RELEASE_NULL(m_pRenderTarget);
	m_pRenderTarget = pRenderTexture;
}

CWVOID cwStage::setRefreshRenderTarget(CWBOOL bRefresh)
{
	m_bRefreshRenderTarget = bRefresh;
}

CWVOID cwStage::addRenderGenerator(cwRenderGenerator* pGenerator)
{
	if (pGenerator) {
		m_nVecGenerator.pushBack(pGenerator);
	}
}

CWVOID cwStage::setCamera(cwCamera* pCamera)
{
	if (m_pCamera == pCamera) return;
	CW_SAFE_RETAIN(pCamera);
	CW_SAFE_RELEASE_NULL(m_pCamera);
	m_pCamera = pCamera;
}

CWVOID cwStage::reset()
{

}

CWVOID cwStage::begin()
{
	reset();

	cwScene* pScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pScene) return;

	for (auto pLayer : m_nVecLayer) {
		eStageLayerFliterType eFilterType = pLayer->getFliterType();

		switch (eFilterType) {
		case eStageLayerFliterStage:
			pLayer->begin(m_nVecStageEntities, m_pStageEffect);
			break;
		case eStageLayerFliterEntity:
		{
			cwVector<cwEntity*>& vecEntity = pScene->getVisibleEntities(nullptr);
			pLayer->begin(vecEntity, m_pStageEffect);
		}
			break;
		case eStageLayerFliterMirror:
		{
			cwVector<cwEntity*>& vecEntity = pScene->getVisibleEntities(nullptr, eSceneObjectMirror);
			pLayer->begin(vecEntity, m_pStageEffect);
		}
			break;
		}
	}
}

CWVOID cwStage::render()
{
	cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pCamera);
	if (m_pStageEffect) {
		//config effect parameter
	}

	cwRepertory::getInstance().getDevice()->setViewPort(m_pViewPort);
	if (m_bRefreshRenderTarget) {
		cwRepertory::getInstance().getDevice()->setRenderTarget(m_pRenderTarget);
		cwRepertory::getInstance().getDevice()->beginDraw(m_bClearColor, m_bClearDepth, m_bClearStencil);
	}

	for (auto pLayer : m_nVecLayer) {
		pLayer->render();
	}
}

CWVOID cwStage::end()
{
	for (auto pLayer : m_nVecLayer) {
		pLayer->end();
	}

	cwRepertory::getInstance().getDevice()->endDraw();

	for (auto pGenerator : m_nVecGenerator) {
		pGenerator->generate();
	}
}

CWVOID cwStage::addStageEntity(cwEntity* pEntity)
{
	if (!pEntity) return;
	m_nVecStageEntities.pushBack(pEntity);
}

CWVOID cwStage::addStageLayer(cwStageLayer* pLayer)
{
	if (!pLayer) return;
	m_nVecLayer.pushBack(pLayer);
}

CWUINT cwStage::getStageLayerCount() const
{
	return static_cast<CWUINT>(m_nVecLayer.size());
}

cwStageLayer* cwStage::getStageLayer(CWUINT index)
{
	if (index >= m_nVecLayer.size()) return nullptr;
	return m_nVecLayer.at(index);
}

CWVOID cwStage::clearStageLayer(CWBOOL bClear)
{
	m_nVecLayer.clear();
}

CWVOID cwStage::clearStageGenerator()
{
	m_nVecGenerator.clear();
}

CWVOID cwStage::clearStageEntity()
{
	m_nVecStageEntities.clear();
}

NS_MINIR_END
