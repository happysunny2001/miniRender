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

#include "cwSkyDomeStage.h"
#include "Entity/cwSkyDome.h"
#include "Entity/cwScene.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Camera/cwCamera.h"

NS_MINIR_BEGIN

cwSkyDomeStage* cwSkyDomeStage::create()
{
	cwSkyDomeStage* pStage = new cwSkyDomeStage();
	if (pStage) {
		pStage->autorelease();
		return pStage;
	}

	return nullptr;
}

cwSkyDomeStage::cwSkyDomeStage():
m_pCamera(nullptr)
{

}

cwSkyDomeStage::~cwSkyDomeStage()
{

}

CWVOID cwSkyDomeStage::reset()
{
	m_nRenderBatch.reset();
}

CWVOID cwSkyDomeStage::begin()
{
	reset();

	cwRepertory& repertory = cwRepertory::getInstance();

	cwCamera* pCamera = repertory.getEngine()->getRenderer()->getRendererCamera();
	cwScene* pScene = repertory.getEngine()->getCurrScene();
	if (pScene && pCamera) {
		cwSkyDome* pSkyDome = pScene->getSkyDome();
		if (pSkyDome) {
			pSkyDome->setPosition(pCamera->getPos());
			pSkyDome->transform();
			pSkyDome->refreshTransform();

			m_nRenderBatch.m_nWorldTrans = pSkyDome->getTransformMatrix();
			m_nRenderBatch.m_pBlend = pSkyDome->getBlend();
			m_nRenderBatch.m_pStencil = pSkyDome->getStencil();
			m_nRenderBatch.m_pEffect = pSkyDome->getEffect();
			m_nRenderBatch.m_pEntity = pSkyDome;
		}
	}

	m_pCamera = repertory.getEngine()->getRenderer()->getCurrCamera();
	repertory.getEngine()->getRenderer()->setCurrCamera(pCamera);
}

CWVOID cwSkyDomeStage::render()
{
	cwRepertory::getInstance().getEngine()->getRenderer()->render(&m_nRenderBatch);
}

CWVOID cwSkyDomeStage::end()
{
	cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pCamera);
}

NS_MINIR_END
