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

#include "cwPUStageLayerLightTrans.h"
#include "Entity/cwScene.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"

NS_MINIR_BEGIN

cwPUStageLayerLightTrans* cwPUStageLayerLightTrans::create()
{
	cwPUStageLayerLightTrans* pPUStageLayer = new cwPUStageLayerLightTrans();
	if (pPUStageLayer) {
		pPUStageLayer->autorelease();
		return pPUStageLayer;
	}

	return nullptr;
}

cwPUStageLayerLightTrans::cwPUStageLayerLightTrans()
{
	m_eType = ePUStageLayerLightTrans;
}

cwPUStageLayerLightTrans::~cwPUStageLayerLightTrans()
{

}

CWVOID cwPUStageLayerLightTrans::begin()
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene) return;

	saveDirectionalLight(pCurrScene);
	savePointLight(pCurrScene);
	saveSpotLight(pCurrScene);

	cwRepertory::getInstance().getEngine()->getRenderer()->configLight();
}

CWVOID cwPUStageLayerLightTrans::saveDirectionalLight(cwScene* pScene)
{
	const cwVector<cwDirectionalLight*>& vecLight = pScene->getDirectionalLights();
	for (auto pLight : vecLight) {
		m_nMapDirectional[pLight] = pLight->getDirection();
		pLight->setDirection(pLight->getDirection()*m_nMatLightTrans);
	}
}

CWVOID cwPUStageLayerLightTrans::savePointLight(cwScene* pScene)
{
	const cwVector<cwPointLight*>& vecLight = pScene->getPointLights();
	for (auto pLight : vecLight) {
		m_nMapPoint[pLight] = pLight->getPosition();
		pLight->setPosition(pLight->getPosition()*m_nMatLightTrans);
	}
}

CWVOID cwPUStageLayerLightTrans::saveSpotLight(cwScene* pScene)
{
	const cwVector<cwSpotLight*>& vecLight = pScene->getSpotLights();
	for (auto pLight : vecLight) {
		m_nMapSpot[pLight] = pLight->getPosition();
		pLight->setPosition(pLight->getPosition()*m_nMatLightTrans);

		cwVector4D vecSpotDir = pLight->getSpotDirection();
		m_nMapSpotDir[pLight] = vecSpotDir;

		float fSpot = vecSpotDir.w;
		vecSpotDir.w = 0;
		vecSpotDir *= m_nMatLightTrans;
		pLight->setSpotDirection(vecSpotDir);
		pLight->setSpotLightExponent(fSpot);
	}
}

CWVOID cwPUStageLayerLightTrans::end()
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (!pCurrScene) return;

	restoreDirectionalLight(pCurrScene);
	restorePointLight(pCurrScene);
	restoreSpotLight(pCurrScene);

	cwRepertory::getInstance().getEngine()->getRenderer()->configLight();
}

CWVOID cwPUStageLayerLightTrans::restoreDirectionalLight(cwScene* pScene)
{
	const cwVector<cwDirectionalLight*>& vecLight = pScene->getDirectionalLights();
	for (auto pLight : vecLight) {
		pLight->setDirection(m_nMapDirectional[pLight]);
	}

	m_nMapDirectional.clear();
}

CWVOID cwPUStageLayerLightTrans::restorePointLight(cwScene* pScene)
{
	const cwVector<cwPointLight*>& vecLight = pScene->getPointLights();
	for (auto pLight : vecLight) {
		pLight->setPosition(m_nMapPoint[pLight]);
	}

	m_nMapPoint.clear();
}

CWVOID cwPUStageLayerLightTrans::restoreSpotLight(cwScene* pScene)
{
	const cwVector<cwSpotLight*>& vecLight = pScene->getSpotLights();
	for (auto pLight : vecLight) {
		pLight->setPosition(m_nMapSpot[pLight]);
		cwVector4D& vecDir = m_nMapSpotDir[pLight];
		pLight->setSpotDirection(vecDir);
		pLight->setSpotLightExponent(vecDir.w);
	}

	m_nMapSpot.clear();
	m_nMapSpotDir.clear();
}

NS_MINIR_END
