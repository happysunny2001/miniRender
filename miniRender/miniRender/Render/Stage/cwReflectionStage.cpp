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

#include "cwReflectionStage.h"
#include "Texture/cwCubeTexture.h"
#include "Texture/cwRenderTexture.h"
#include "ViewPort/cwViewPort.h"
#include "Camera/cwCamera.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Engine/cwEngine.h"
#include "Entity/cwRenderNode.h"
#include "Entity/cwReflectionEntity.h"
#include "Render/cwRenderer.h"
#include "cwStageLayer.h"

NS_MINIR_BEGIN

cwReflectionStage* cwReflectionStage::create()
{
	cwReflectionStage* pStage = new cwReflectionStage();
	if (pStage) {
		pStage->autorelease();
		return pStage;
	}

	return nullptr;
}

cwReflectionStage::cwReflectionStage():
m_pCubeTexture(nullptr),
m_pViewport(nullptr),
m_pPrevViewPort(nullptr)
{
	m_eType = eStageTypeReflection;
	buildCameras();
}

cwReflectionStage::~cwReflectionStage()
{
	//CW_SAFE_RELEASE_NULL(m_pCubeTexture);
	CW_SAFE_RELEASE_NULL(m_pViewport);

	for (CWUINT i = 0; i < 6; ++i) {
		if (m_nCameras[i]) {
			CW_SAFE_RELEASE_NULL(m_nCameras[i]);
		}
	}

	m_pPrevViewPort = nullptr;
}

CWVOID cwReflectionStage::buildCameras()
{
	for (CWUINT i = 0; i < 6; ++i) {
		m_nCameras[i] = cwCamera::create();
		CW_SAFE_RETAIN(m_nCameras[i]);

		m_nCameras[i]->updateProjMatrixFov(cwMathUtil::cwPI*0.5f, 1.0f, 0.1f, 1000.0f);
	}
}

CWVOID cwReflectionStage::updateCamera(const cwVector3D& pos)
{
	static cwVector3D up[6] = {
		cwVector3D(0.0f, 1.0f, 0.0f), //+X
		cwVector3D(0.0f, 1.0f, 0.0f), //-X
		cwVector3D(0.0f, 0.0f, -1.0f), //+Y
		cwVector3D(0.0f, 1.0f, 1.0f), //-Y
		cwVector3D(0.0f, 1.0f, 0.0f), //+Z
		cwVector3D(0.0f, 1.0f, 0.0f), //-Z
	};

	cwVector3D targets[6] = {
		cwVector3D(pos.x + 1.0f, pos.y, pos.z), //+X
		cwVector3D(pos.x - 1.0f, pos.y, pos.z), //-X
		cwVector3D(pos.x, pos.y + 1.0f, pos.z), //+Y
		cwVector3D(pos.x, pos.y - 1.0f, pos.z), //-Y
		cwVector3D(pos.x, pos.y, pos.z + 1.0f), //+Z
		cwVector3D(pos.x, pos.y, pos.z - 1.0f), //-Z
	};

	for (CWUINT i = 0; i < 6; ++i) {
		m_nCameras[i]->lookAt(pos, targets[i], up[i]);
	}
}

//CWVOID cwReflectionStage::setSize(CWUINT iSize)
//{
//	//CW_SAFE_RELEASE_NULL(m_pCubeTexture);
//	CW_SAFE_RELEASE_NULL(m_pViewport);
//
//	//m_pCubeTexture = cwRepertory::getInstance().getDevice()->createCubeTexture(iSize);
//	//CW_SAFE_RETAIN(m_pCubeTexture);
//
//	m_pViewport = cwRepertory::getInstance().getDevice()->createViewPort(0.0f, 0.0f, (CWFLOAT)iSize, (CWFLOAT)iSize);
//	CW_SAFE_RETAIN(m_pViewport);
//}

CWVOID cwReflectionStage::reset()
{
	if (!m_pCubeTexture) {
		if (m_nMapStageTextures.size() > 0)
			m_pCubeTexture = static_cast<cwCubeTexture*>(m_nMapStageTextures.begin()->second);
	}

	if (!m_pViewport && m_pCubeTexture) {
		m_pViewport = cwRepertory::getInstance().getDevice()->createViewPort(0.0f, 0.0f, m_pCubeTexture->getWidth(), m_pCubeTexture->getWidth());
		CW_SAFE_RETAIN(m_pViewport);
	}

	if (m_pCubeTexture)
		m_pCubeTexture->setActiveCubeFace(eCubeFaceRight);
}

CWVOID cwReflectionStage::begin()
{
	reset();

	if (!m_pCubeTexture) return;

	m_pPrevViewPort = cwRepertory::getInstance().getDevice()->getViewPort();

	if (!m_nVecStage.empty()) {
		m_pRenderTarget = m_nVecStage.back()->getRenderTexture();
		m_nVecStage.back()->setRenderTexture(m_pCubeTexture);

		for (auto pStage : m_nVecStage) {
			m_nVecStageCameras.push_back(pStage->getCamera());
		}
	}
}

CWVOID cwReflectionStage::render()
{
	if (!m_pCubeTexture) return;

	m_nVecRenderNodes.clear();
	cwRepertory::getInstance().getEngine()->getVisibleNodes(m_pCamera, eRenderTypeReflection, m_nVecRenderNodes);
	if (m_nVecRenderNodes.empty()) return;

	for (auto pNode : m_nVecRenderNodes) {
		cwRepertory::getInstance().getDevice()->setViewPort(m_pViewport);
		updateCamera(pNode->getPosition());

		for (CWUINT i = 0; i < eCubeFaceMax; ++i) {
			m_pCubeTexture->setActiveCubeFace((eCubeTextureFace)i);

			for (auto pStage : m_nVecStage) {
				pStage->setCamera(m_nCameras[i]);

				pStage->begin();
				pStage->render();
				pStage->end();
			}
		}

		cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pCamera);
		cwRepertory::getInstance().getDevice()->setViewPort(m_pPrevViewPort);
		cwRepertory::getInstance().getDevice()->setRenderTarget(m_pRenderTarget);
		cwRepertory::getInstance().getDevice()->beginDraw(m_bClearColor, m_bClearDepth, m_bClearStencil);

		cwReflectionEntity* pReflectionEntity = static_cast<cwReflectionEntity*>(pNode);
		m_pCubeTexture->generateMips();
		pReflectionEntity->setDynamicRelfectionTexture(m_pCubeTexture);

		for (auto pLayer : m_nVecLayer) {
			pLayer->begin(pNode);
			pLayer->render();
			pLayer->end();
		}
	}
}

CWVOID cwReflectionStage::end()
{
	if (!m_pCubeTexture) return;

	cwRepertory::getInstance().getDevice()->setViewPort(m_pPrevViewPort);
	m_pPrevViewPort = nullptr;

	if (!m_nVecStage.empty()) {
		m_nVecStage.back()->setRenderTexture(m_pRenderTarget);
		m_pRenderTarget = nullptr;

		for (CWUINT i = 0; i < (CWUINT)m_nVecStage.size(); ++i) {
			m_nVecStage[i]->setCamera(m_nVecStageCameras[i]);
		}

		m_nVecStageCameras.clear();
	}
}

CWVOID cwReflectionStage::addStage(cwStage* pStage)
{
	if (pStage) {
		m_nVecStage.push_back(pStage);
	}
}

NS_MINIR_END
