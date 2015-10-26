﻿/*
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

#include "cwScene.h"
#include "Base/cwMacros.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "cwEntity.h"
#include "cwSkyDome.h"

NS_MINIR_BEGIN

cwScene* cwScene::create()
{
	cwScene* pScene = new cwScene();
	if (pScene && pScene->init()) {
		pScene->autorelease();
		return pScene;
	}
	CW_SAFE_DELETE(pScene);
	return nullptr;
}

cwScene::cwScene():
m_pSkyDome(nullptr)
{
	m_eType = eSceneObjectScene;
}

cwScene::~cwScene()
{
	CW_SAFE_RELEASE_NULL(m_pSkyDome);
}

CWBOOL cwScene::init()
{
	if (!cwRenderNode::init()) return CWFALSE;

	return CWTRUE;
}

CWVOID cwScene::addDirectionalLight(cwDirectionalLight* pLight)
{
	if (!pLight) return;
	if (m_nVecDirectionalLights.contains(pLight)) return;
	m_nVecDirectionalLights.pushBack(pLight);
}

CWVOID cwScene::addPointLight(cwPointLight* pLight)
{
	if (!pLight) return;
	if (m_nVecPointLights.contains(pLight)) return;
	m_nVecPointLights.pushBack(pLight);
}

CWVOID cwScene::addSpotLight(cwSpotLight* pLight)
{
	if (!pLight) return;
	if (m_nVecSpotLights.contains(pLight)) return;
	m_nVecSpotLights.pushBack(pLight);
}

CWVOID cwScene::removeDirectionalLight(cwDirectionalLight* pLight)
{
	if (!pLight) return;
	m_nVecDirectionalLights.erase(pLight);
}

CWVOID cwScene::removePointLight(cwPointLight* pLight)
{
	if (!pLight) return;
	m_nVecPointLights.erase(pLight);
}

CWVOID cwScene::removeSpotLight(cwSpotLight* pLight)
{
	if (!pLight) return;
	m_nVecSpotLights.erase(pLight);
}

const cwVector<cwDirectionalLight*>& cwScene::getDirectionalLights() const
{
	return m_nVecDirectionalLights;
}

const cwVector<cwPointLight*>& cwScene::getPointLights() const
{
	return m_nVecPointLights;
}

const cwVector<cwSpotLight*>& cwScene::getSpotLights() const
{
	return m_nVecSpotLights;
}

CWBOOL cwScene::createSkyDome(const std::string& strSkyTexture)
{
	CW_SAFE_RELEASE_NULL(m_pSkyDome);
	m_pSkyDome = cwSkyDome::create(strSkyTexture);
	CW_SAFE_RETAIN(m_pSkyDome);
	if (!m_pSkyDome) return CWFALSE;

	return CWTRUE;
}

//CWVOID cwScene::getRenderNode(eSceneObjectType eType, std::vector<cwRenderNode*>& vecNodes)
//{
//	std::vector<cwRenderNode*> nVecStack;
//	nVecStack.reserve(10);
//	nVecStack.push_back(this);
//
//	while (!nVecStack.empty()) {
//		cwRenderNode* pLast = nVecStack.back();
//		if (pLast && (pLast->getType() & eType) && pLast->getVisible()) {
//			vecNodes.push_back(pLast);
//		}
//
//		nVecStack.pop_back();
//
//		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
//		if (!nVecChildren.empty()) {
//			for (auto pNode : nVecChildren) {
//				nVecStack.push_back(pNode);
//			}
//		}
//	}
//}

//cwVector<cwEntity*>& cwScene::getVisibleEntities(cwCamera* pCamera, eSceneObjectType eType)
//{
//	m_nVecVisibleEntity.clear();
//
//	std::vector<cwRenderNode*> m_nVecStack;
//	m_nVecStack.push_back(this);
//
//	while (!m_nVecStack.empty()) {
//		cwRenderNode* pLast = m_nVecStack.back();
//		if (pLast->getType() == eType && pLast->getVisible())
//			m_nVecVisibleEntity.pushBack(static_cast<cwEntity*>(pLast));
//
//		m_nVecStack.pop_back();
//
//		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
//		if (!nVecChildren.empty()) {
//			for (auto pNode : nVecChildren) {
//				m_nVecStack.push_back(pNode);
//			}
//		}
//	}
//
//	return m_nVecVisibleEntity;
//}

NS_MINIR_END
