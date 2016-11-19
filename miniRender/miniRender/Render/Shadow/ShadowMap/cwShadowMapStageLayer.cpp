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
#include "Texture/cwTexture.h"
#include "Camera/cwOrthoCamera.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Entity/cwRenderNode.h"
#include "Entity/cwScene.h"
#include "Device/cwDevice.h"
#include "ViewPort/cwViewPort.h"
#include "Shader/cwShader.h"

NS_MINIR_BEGIN

cwShadowMapStageLayer::cwShadowMapStageLayer() : 
m_bHasLightingShadow(CWFALSE),
m_pShadowCamera(nullptr)
{
	m_nMatTextureSpaceTrans.set(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}

cwShadowMapStageLayer::~cwShadowMapStageLayer()
{
	CW_SAFE_RELEASE_NULL(m_pShadowCamera);
}

CWBOOL cwShadowMapStageLayer::init()
{
	if (!cwStageLayer::init()) return CWFALSE;
	if (!buildLightSourceCamera()) return CWFALSE;

	return CWTRUE;
}

CWVOID cwShadowMapStageLayer::setDepthStencil(cwTexture* pTexture)
{
	cwStageLayer::setDepthStencil(pTexture);

	if (m_pDepthStencil) {
		this->buildViewPort(0, 0, m_pDepthStencil->getWidth(), m_pDepthStencil->getHeight(), 0, 1.0);
	}
}

CWVOID cwShadowMapStageLayer::batchSceneRenderNodes()
{
	m_nAabbShadowSpace.zero();
	m_nVecSceneRenderNodes.clear();
	if (!m_pCamera) return;

	cwRepertory::getInstance().getEngine()->getVisibleNodes(m_pCamera, eRenderTypeEntity, m_nVecSceneRenderNodes);
	
	for (auto it = m_nVecSceneRenderNodes.begin(); it != m_nVecSceneRenderNodes.end(); ++it) {
		if ((*it)->getCastShadow()) {
			this->addRenderNodeWithUniformEffect((*it));
			//m_nAabbShadowSpace.add((*it)->getBoundingBox());
		}

		m_nAabbShadowSpace.add((*it)->getBoundingBox());
	}

	refreshLightSourceCamera();

	//m_nVecSceneRenderNodes.clear();
	//cwRepertory::getInstance().getEngine()->getVisibleNodes(m_pShadowCamera, eRenderTypeEntity, m_nVecSceneRenderNodes);
	//m_nVecSceneRenderNodes.clear();
}

CWVOID cwShadowMapStageLayer::begin()
{
	this->refreshShadowCastState();

	if (m_bHasLightingShadow) {
		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
		pDevice->pushDeviceStatus();

		cwStageLayer::begin();
	}
}

CWVOID cwShadowMapStageLayer::render()
{
	if (m_bHasLightingShadow) {
		cwStageLayer::render();
	}
}

CWVOID cwShadowMapStageLayer::end()
{
	if (m_bHasLightingShadow) {
		cwStageLayer::end();
		cwRepertory::getInstance().getDevice()->popDeviceStatus();
	}
}

CWVOID cwShadowMapStageLayer::refreshShadowCastState()
{
	m_bHasLightingShadow = CWFALSE;

	cwScene* pScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pScene) return;

	cwDirectionalLight* pLight = pScene->getDirectionalLight();
	if (!pLight || !pLight->getCastShadow()) return;

	m_bHasLightingShadow = CWTRUE;
}

CWBOOL cwShadowMapStageLayer::buildLightSourceCamera()
{
	m_pShadowCamera = cwOrthoCamera::create();
	if (!m_pShadowCamera) return CWFALSE;
	CW_SAFE_RETAIN(m_pShadowCamera);

	return CWTRUE;
}

CWVOID cwShadowMapStageLayer::refreshLightSourceCamera()
{
	static cwVector3D up(0, 1.0f, 0);

	cwScene* pScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	cwDirectionalLight* pLight = pScene->getDirectionalLight();

	cwVector3D centerPos = m_nAabbShadowSpace.center();
	CWFLOAT radius = m_nAabbShadowSpace.radius().length();

	const cwVector4D& lightDir = pLight->getDirection();
	cwVector3D cameraPos = -2.0f*radius*cwVector3D(lightDir);
	m_pShadowCamera->lookAt(cameraPos, centerPos, up);

	cwVector3D centerView = cwVector4D(centerPos, 1.0f) * m_pShadowCamera->getViewMatrix();
	CWFLOAT l = centerView.x - radius;
	CWFLOAT r = centerView.x + radius;
	CWFLOAT t = centerView.y + radius;
	CWFLOAT b = centerView.y - radius;
	CWFLOAT n = centerView.z - radius;
	CWFLOAT f = centerView.z + radius;

	m_pShadowCamera->updateProjMatrix(l, r, t, b, n, f);
	m_nMatShadowTransform = m_pShadowCamera->getViewProjMatrix()*m_nMatTextureSpaceTrans;
}

CWVOID cwShadowMapStageLayer::preFrameConfig(cwShader* pShader)
{
	cwStageLayer::preFrameConfig(pShader);

	if (pShader && m_pShadowCamera) {
		pShader->setVariableMatrix("gMatShadowViewProj", m_pShadowCamera->getViewProjMatrix());
	}
}

NS_MINIR_END
