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

#include "cwShadowMapStageLayer.h"
#include "effect/cwEffect.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Entity/cwScene.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwShadowMapStageLayer* cwShadowMapStageLayer::create()
{
	cwShadowMapStageLayer* pStageLayer = new cwShadowMapStageLayer();
	if (pStageLayer) {
		pStageLayer->autorelease();
		return pStageLayer;
	}

	return nullptr;
}

cwShadowMapStageLayer::cwShadowMapStageLayer() :
m_pShadowEffect(nullptr)
{
	m_nLayerType = "ShadowMap";
	m_pOrthoCamera = cwOrthoCamera::create();
	CW_SAFE_RETAIN(m_pOrthoCamera);

	buildShadowMapEffect();

	m_nMatTextureSpaceTrans.set(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}

cwShadowMapStageLayer::~cwShadowMapStageLayer()
{
	CW_SAFE_RELEASE_NULL(m_pShadowEffect);
	CW_SAFE_RELEASE_NULL(m_pOrthoCamera);
}

CWVOID cwShadowMapStageLayer::addEntities(std::vector<cwRenderNode*>* vecEntities)
{
	cwRenderPipeline* pPiepline = getPipeline();
	if (!pPiepline) {
		return;
	}

	pPiepline->reset();
	pPiepline->setShader(m_pShadowEffect->getShader());

	for (auto it = vecEntities->begin(); it != vecEntities->end(); ++it) {
		if (!(*it)->getCastShadow()) continue;

		pPiepline->addEntity(*it, m_pShadowEffect, nullptr, nullptr, this);

		if (pPiepline->full()) {
			pPiepline = getPipeline();
			if (!pPiepline) return;

			pPiepline->reset();
			pPiepline->setShader(m_pShadowEffect->getShader());
		}
	}

	if (m_iPipeLineIndex == 0 && m_nPipeline[0].getBatchCount() == 0) {
		return;
	}

	++m_iPipeLineIndex;
}

cwRenderPipeline* cwShadowMapStageLayer::getPipeline()
{
	if (m_iPipeLineIndex >= CW_STAGE_PIPELINE_SIZE) return nullptr;

	cwRenderPipeline* pPipeline = &(m_nPipeline[m_iPipeLineIndex]);
	if (pPipeline->full()) {
		++m_iPipeLineIndex;
		if (m_iPipeLineIndex >= CW_STAGE_PIPELINE_SIZE) return nullptr;
		return &(m_nPipeline[m_iPipeLineIndex]);
	}

	return pPipeline;
}

CWVOID cwShadowMapStageLayer::buildShadowMapEffect()
{
	m_pShadowEffect = cwEffect::create();
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->createShader("shadowMap.fx");
	m_pShadowEffect->setShader(pShader);
	CW_SAFE_RETAIN(m_pShadowEffect);
}

//CWVOID cwShadowMapStageLayer::buildLightTransform()
//{
//	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
//	if (!pCurrScene) return;
//	const cwVector<cwDirectionalLight*>& vecLight = pCurrScene->getDirectionalLights();
//	if (vecLight.empty()) {
//		return;
//	}
//
//	cwDirectionalLight* light = vecLight.at(0);
//	const cwVector4D& lightDir = light->getDirection();
//	cwVector3D pos = -2.0f*50.0f*cwVector3D(lightDir);
//	m_pOrthoCamera->lookAt(pos, cwVector3D::ZERO, cwVector3D(0, 1.0, 0));
//	m_pOrthoCamera->updateProjMatrix(100, 100, 1, 300);
//
//	const cwMatrix4X4 matTexture(
//		0.5f,  0.0f, 0.0f, 0.0f,
//		0.0f, -0.5f, 0.0f, 0.0f,
//		0.0f,  0.0f, 1.0f, 0.0f,
//		0.5f,  0.5f, 0.0f, 1.0f);
//
//	m_nMatShadowTransform = m_pOrthoCamera->getViewProjMatrix()*matTexture;
//}

CWVOID cwShadowMapStageLayer::render()
{
	//buildLightTransform();
	//cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pOrthoCamera);
	//cwStageLayer::render();

	renderDirectionalLightShadow();
}

CWVOID cwShadowMapStageLayer::buildDirectionalLightTransform(cwDirectionalLight* pLight)
{
	if (!pLight) return;

	const cwVector4D& lightDir = pLight->getDirection();
	cwVector3D pos = -2.0f*50.0f*cwVector3D(lightDir);
	m_pOrthoCamera->lookAt(pos, cwVector3D::ZERO, cwVector3D(0, 1.0, 0));
	m_pOrthoCamera->updateProjMatrix(100, 100, 1, 300);

	m_nMatShadowTransform = m_pOrthoCamera->getViewProjMatrix()*m_nMatTextureSpaceTrans;
}

CWVOID cwShadowMapStageLayer::renderDirectionalLightShadow()
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene) return;
	const cwVector<cwDirectionalLight*>& vecLight = pCurrScene->getDirectionalLights();
	if (vecLight.empty()) {
		return;
	}

	for (auto pLight : vecLight) {
		if (!pLight->getCastShadow()) continue;
		buildDirectionalLightTransform(pLight);

		cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pOrthoCamera);
		cwStageLayer::render();
	}
}

NS_MINIR_END
