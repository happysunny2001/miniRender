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

#include "cwScene.h"
#include "Base/cwMacros.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "cwEntity.h"
#include "cwSkyDome.h"
#include "Sprite/cwRenderNode2D.h"

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
m_pSkyDome(nullptr),
m_pRootNode2D(nullptr)
{
	m_eRenderType = eRenderTypeScene;
}

cwScene::~cwScene()
{
	CW_SAFE_RELEASE_NULL(m_pSkyDome);
	CW_SAFE_RELEASE_NULL(m_pRootNode2D);
}

CWBOOL cwScene::init()
{
	if (!cwRenderNode::init()) return CWFALSE;
	if (!buildRootNode2D()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwScene::buildRootNode2D()
{
	m_pRootNode2D = cwRenderNode2D::create();
	if (!m_pRootNode2D) return CWFALSE;
	CW_SAFE_RETAIN(m_pRootNode2D);

	m_nVecNode2DStack.reserve(100);
	m_nVecDirty2DStack.reserve(100);
	m_nVecRefresh2DStack.reserve(50);

	return CWTRUE;
}

CWVOID cwScene::addChild2D(cwRenderNode2D* pNode2D)
{
	m_pRootNode2D->addChild(pNode2D);
}

CWVOID cwScene::removeChild2D(cwRenderNode2D* pNode2D)
{
	m_pRootNode2D->removeChild(pNode2D);
}

std::vector<cwRenderNode2D*>& cwScene::getRenderNodes2D()
{
	refreshNode2D();

	m_nVecRender2DQueue.clear();

	m_nVecRender2DQueue.push_back(m_pRootNode2D);
	for (CWUINT64 index = 0; index < m_nVecRender2DQueue.size(); ++index) {
		cwRenderNode2D* pNode2D = m_nVecRender2DQueue[index];

		if (!pNode2D || !pNode2D->getVisible()) continue;

		cwVector<cwRenderNode*>& nVecChildren = pNode2D->getChildren();
		if (!nVecChildren.empty()) {
			for (auto it = nVecChildren.begin(); it != nVecChildren.end(); ++it) {
				m_nVecRender2DQueue.push_back(static_cast<cwRenderNode2D*>(*it));
			}
		}
	}

	return m_nVecRender2DQueue;
}

CWVOID cwScene::refreshNode2D()
{
	m_nVecNode2DStack.clear();
	m_nVecDirty2DStack.clear();

	m_nVecNode2DStack.push_back(m_pRootNode2D);

	while (!m_nVecNode2DStack.empty()) {
		cwRenderNode2D* pLastNode2D = m_nVecNode2DStack.back();
		m_nVecNode2DStack.pop_back();

		if (!pLastNode2D->getVisible()) continue;

		if (pLastNode2D->getTransDirty()) {
			refreshDirtyNode2D(pLastNode2D);
		}

		cwVector<cwRenderNode*>& nVecChildren = pLastNode2D->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pChildNode : nVecChildren) {
				m_nVecNode2DStack.push_back(static_cast<cwRenderNode2D*>(pChildNode));
			}
		}
	}
}

CWVOID cwScene::refreshDirtyNode2D(cwRenderNode2D* pNode2D)
{
	m_nVecDirty2DStack.clear();
	m_nVecRefresh2DStack.clear();

	m_nVecRefresh2DStack.push_back(pNode2D);
	while (!m_nVecRefresh2DStack.empty()) {
		cwRenderNode2D* pLastNode2D = m_nVecRefresh2DStack.back();
		m_nVecRefresh2DStack.pop_back();

		if (pLastNode2D->getTransDirty()) {
			pLastNode2D->transform();
		}

		m_nVecDirty2DStack.push_back(pLastNode2D);

		cwVector<cwRenderNode*>& nVecChildren = pLastNode2D->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pChildNode : nVecChildren) {
				if (pChildNode && pChildNode->getVisible())
					m_nVecRefresh2DStack.push_back(static_cast<cwRenderNode2D*>(pChildNode));
			}
		}
	}

	for (auto pNode : m_nVecDirty2DStack) {
		pNode->refreshTransform();
		pNode->refreshBoundingBox();
	}
}

CWVOID cwScene::addDirectionalLight(cwDirectionalLight* pLight)
{
	//if (m_nVecDirectionalLights.contains(pLight)) return;
	//m_nVecDirectionalLights.pushBack(pLight);
	CW_SAFE_RETAIN(pLight);
	CW_SAFE_RELEASE(m_pDirectionalLight);
	m_pDirectionalLight = pLight;
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

CWVOID cwScene::removeDirectionalLight()
{
	CW_SAFE_RELEASE_NULL(m_pDirectionalLight);
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

cwDirectionalLight* cwScene::getDirectionalLight() const
{
	return m_pDirectionalLight;
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
