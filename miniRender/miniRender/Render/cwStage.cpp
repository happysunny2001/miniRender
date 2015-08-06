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

NS_MINIR_BEGIN

cwStage::cwStage() : 
m_pCamera(nullptr),
m_bEnable(CWTRUE),
m_eType(eStageTypeNormal),
m_pViewPort(nullptr),
m_pRenderTarget(nullptr),
m_pStageEffect(nullptr)
{

}

cwStage::~cwStage()
{
	CW_SAFE_RELEASE_NULL(m_pCamera);
	CW_SAFE_RELEASE_NULL(m_pViewPort);
	CW_SAFE_RELEASE_NULL(m_pRenderTarget);
	CW_SAFE_RELEASE_NULL(m_pStageEffect);
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

CWVOID cwStage::reset()
{
	m_iPipeLineIndex = 0;
	m_nMapPipeline.clear();
}

CWVOID cwStage::begin()
{
	reset();

	cwScene* pScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pScene) return;

	std::vector<cwEntity*>& vecEntity = pScene->getVisibleEntities(nullptr);

	if (m_pStageEffect) {
		cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
		pPipeline->reset();

		for (auto pEntity : vecEntity) {
			pEntity->transform();

			CWBOOL bRet = pPipeline->addEntity(pEntity, nullptr);
			if (!bRet) {
				if (m_iPipeLineIndex >= CW_STAGE_PIPELINE_SIZE)
					break;
				else {
					pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
					pPipeline->reset();
				}
			}
		}
	}
	else {
		cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
		pPipeline->reset();

		for (auto pEntity : vecEntity) {
			pEntity->transform();

			CWBOOL bRet = pPipeline->addEntity(pEntity);
			if (!bRet) {
				if (m_iPipeLineIndex >= CW_STAGE_PIPELINE_SIZE)
					break;
				else {
					pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
					pPipeline->reset();
				}
			}
		}
	}

	cwRepertory::getInstance().getEngine()->setCurrCamera(m_pCamera);
	if (m_pStageEffect) {
		cwRepertory::getInstance().getEngine()->setCurrShader(m_pStageEffect->getShader());
	}

	cwRepertory::getInstance().getDevice()->setViewPort(m_pViewPort);
	cwRepertory::getInstance().getDevice()->setRenderTarget(m_pRenderTarget);
	cwRepertory::getInstance().getDevice()->beginDraw();
}

CWVOID cwStage::render()
{
	for (CWUINT i = 0; i < m_iPipeLineIndex; ++i) {
		m_nPipeline[i].render();
	}
}

CWVOID cwStage::end()
{
	cwRepertory::getInstance().getDevice()->endDraw();
}

cwRenderPipeline* cwStage::getPipeline(cwEntity* pEntity)
{
	cwShader* pShader = pEntity->getEffect()->getShader();
	if (m_nMapPipeline.find(pShader) != m_nMapPipeline.end()) {
		return m_nMapPipeline[pShader];
	}
	else {
		if (m_iPipeLineIndex < CW_STAGE_PIPELINE_SIZE) {
			cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex++]);
			m_nMapPipeline[pShader] = pPipeline;
			return pPipeline;
		}
	}

	return nullptr;
}

NS_MINIR_END
