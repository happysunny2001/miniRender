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
#include "cwEntity.h"

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

cwScene::cwScene()
{
	m_eType = eSceneObjectScene;
}

cwScene::~cwScene()
{

}

CWBOOL cwScene::init()
{
	if (!cwRenderNode::init()) return CWFALSE;
	return CWTRUE;
}

CWVOID cwScene::addLight(cwLight* pLight)
{
	if (!pLight) return;
	if (m_nVecLights.contains(pLight)) return;
	m_nVecLights.pushBack(pLight);
}

CWVOID cwScene::removeLight(cwLight* pLight)
{
	if (!pLight) return;
	m_nVecLights.erase(pLight);
}

const cwVector<cwLight*>& cwScene::getLights() const
{
	return m_nVecLights;
}

std::vector<cwEntity*>& cwScene::getVisibleEntities(cwCamera* pCamera)
{
	m_nVecVisibleEntity.clear();

	std::vector<cwRenderNode*> m_nVecStack;
	m_nVecStack.push_back(this);

	while (!m_nVecStack.empty()) {
		cwRenderNode* pLast = m_nVecStack.back();
		if (pLast->getType() == eSceneObjectEntity)
			m_nVecVisibleEntity.push_back(static_cast<cwEntity*>(pLast));

		m_nVecStack.pop_back();

		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pNode : nVecChildren) {
				m_nVecStack.push_back(pNode);
			}
		}
	}

	return m_nVecVisibleEntity;
}

NS_MINIR_END
