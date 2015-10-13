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

#include "cwSpriteManager.h"
#include "cwSprite.h"
#include "Base/cwStruct.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Entity/cwScene.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Render/cwRenderer.h"
#include "effect/cwEffect.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Device/cwDevice.h"
#include "Stencil/cwStencil.h"

NS_MINIR_BEGIN

cwSpriteManager* cwSpriteManager::create()
{
	cwSpriteManager* pSpriteManager = new cwSpriteManager();
	if (pSpriteManager && pSpriteManager->init()) {
		pSpriteManager->autorelease();
		return pSpriteManager;
	}

	CW_SAFE_DELETE(pSpriteManager);
	return nullptr;
}

cwSpriteManager::cwSpriteManager():
m_pRenderObjects(nullptr),
m_pDefEffect(nullptr),
m_pRootSprite(nullptr)
{

}

cwSpriteManager::~cwSpriteManager()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObjects);
	CW_SAFE_RELEASE_NULL(m_pDefEffect);
	CW_SAFE_RELEASE_NULL(m_pRootSprite);
}

CWBOOL cwSpriteManager::init()
{
	if (!buildRenderObjects()) return CWFALSE;
	if (!buildEffect()) return CWFALSE;

	m_pRootSprite = cwRenderNode::create();
	CW_SAFE_RETAIN(m_pRootSprite);

	return CWTRUE;
}

CWBOOL cwSpriteManager::buildRenderObjects()
{
	memset(m_nVertexBuffer, 0, sizeof(m_nVertexBuffer));
	m_pRenderObjects = cwDynamicRenderObject::create(ePrimitiveTypeTriangleList, m_nVertexBuffer, sizeof(cwVertexPosTexColor), 1000, NULL, 0, "PosTexColor");
	if (!m_pRenderObjects) return CWFALSE;
	CW_SAFE_RETAIN(m_pRenderObjects);

	return CWTRUE;
}

CWBOOL cwSpriteManager::buildEffect()
{
	m_pDefEffect = cwEffect::create();
	if (!m_pDefEffect) return CWFALSE;
	CW_SAFE_RETAIN(m_pDefEffect);

	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderPosTexColor);
	if (!pShader) return CWFALSE;
	m_pDefEffect->setShader(pShader);

	return CWTRUE;
}

CWVOID cwSpriteManager::begin()
{
	m_nVecSprites.clear();

	cwRepertory& repertory = cwRepertory::getInstance();
	cwScene* pScene = repertory.getEngine()->getCurrScene();
	if (pScene) {
		pScene->getRenderNode(eSceneObjectSprite, m_nVecSprites);
	}

	cwCamera* pOrthoCamera = repertory.getEngine()->getCamera("Ortho");
	m_pCurrCamera = cwRepertory::getInstance().getEngine()->getRenderer()->getCurrCamera();
	repertory.getEngine()->getRenderer()->setCurrCamera(pOrthoCamera);

	repertory.getDevice()->disableZBuffer();
}

CWVOID cwSpriteManager::render()
{
	if (m_nVecSprites.empty()) return;
	m_pDefEffect->config();

	for (auto pNode : m_nVecSprites) {
		if (!pNode) continue;

		m_uVertexCnt = 0;

		cwSprite* pSprite = static_cast<cwSprite*>(pNode);
		pSprite->transform();
		pSprite->refreshTransform();
		pSprite->refreshBoundingBox();
		const cwVertexPosTexColor* pSpriteVertexBuffer = pSprite->getVertexBuffer();
		if (pSpriteVertexBuffer) {
			CWUINT iCnt = pSprite->getVertexCnt();
			for (CWUINT i = 0; i < iCnt; ++i) {
				m_nVertexBuffer[m_uVertexCnt++] = pSpriteVertexBuffer[i];
			}

			renderBatch(pSprite);
		}
	}
}

CWVOID cwSpriteManager::renderBatch(cwSprite* pSprite)
{
	cwRepertory& repertory = cwRepertory::getInstance();
	cwCamera* pOrthoCamera = repertory.getEngine()->getCamera("Ortho");

	m_pRenderObjects->updateVertexData(m_nVertexBuffer, m_uVertexCnt*sizeof(cwVertexPosTexColor));

	repertory.getDevice()->setBlend(pSprite->getBlend());
	repertory.getDevice()->setStencil(pSprite->getStencil());
	repertory.getDevice()->setShaderWorldTrans(m_pDefEffect->getShader(), pSprite->getTransformMatrix(), pOrthoCamera);
	m_pDefEffect->getShader()->setVariableTexture(eShaderParamTexture0, pSprite->getTexture());
	repertory.getDevice()->draw(m_pDefEffect->getShader(), m_pDefEffect->getTech(), m_pRenderObjects);
}

CWVOID cwSpriteManager::end()
{
	cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pCurrCamera);
	cwRepertory::getInstance().getDevice()->enableZBuffer();
	m_nVecSprites.clear();
}

CWVOID cwSpriteManager::refreshSprite()
{
	m_nDirtyNodes.clear();

	std::vector<cwRenderNode*> vecStack;
	vecStack.reserve(10);
	vecStack.push_back(m_pRootSprite);

	while (!vecStack.empty()) {

	}
}

CWVOID cwSpriteManager::addRefreshNode(cwRenderNode* pNode)
{
	std::vector<cwRenderNode*> vecStack;
	vecStack.reserve(10);
	vecStack.push_back(pNode);

	while (!vecStack.empty()) {
		cwRenderNode* pLast = vecStack.back();
		vecStack.pop_back();

		m_nDirtyNodes.push_back(pLast);

		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pChildNode : nVecChildren) {
				if (pChildNode)
					vecStack.push_back(pChildNode);
			}
		}
	}
}

NS_MINIR_END
