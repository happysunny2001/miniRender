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
#include "cwRenderNode2D.h"
#include "Base/cwStruct.h"
#include "Base/cwLog.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Entity/cwScene.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "RenderObject/cwStaticRenderObject.h"
#include "Render/cwRenderer.h"
#include "effect/cwEffect.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderManager.h"
#include "Device/cwDevice.h"
#include "Stencil/cwStencil.h"
#include "Blend/cwBlend.h"
#include "Generator/cwGeometryGenerator.h"

#include <algorithm>
#include <queue>
#include <stack>

NS_MINIR_BEGIN

#define CW_DEF_SPRITE_BUFFER_CNT 1000

static bool compRenderOrder(cwRenderNode2D* pLeft, cwRenderNode2D* pRight)
{
	return pLeft->getRenderOrder() < pRight->getRenderOrder();
}

static bool compRenderOrderInvert(cwRenderNode2D* pLeft, cwRenderNode2D* pRight)
{
	return pLeft->getRenderOrder() > pRight->getRenderOrder();
}

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
m_pRootSprite(nullptr),
m_uVertexCnt(0),
m_pCurrCamera(nullptr),
m_pAlphaBlend(nullptr),
m_pOrthoCamera(nullptr)
//m_pSpriteRenderObject(nullptr)
{

}

cwSpriteManager::~cwSpriteManager()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObjects);
	CW_SAFE_RELEASE_NULL(m_pDefEffect);
	CW_SAFE_RELEASE_NULL(m_pRootSprite);
	CW_SAFE_RELEASE_NULL(m_pAlphaBlend);
	//CW_SAFE_RELEASE_NULL(m_pSpriteRenderObject);
}

CWBOOL cwSpriteManager::init()
{
	if (!buildRenderObjects()) return CWFALSE;
	if (!buildEffect()) return CWFALSE;
	if (!buildBlend()) return CWFALSE;

	m_pRootSprite = cwRenderNode2D::create();
	CW_SAFE_RETAIN(m_pRootSprite);
	m_nDirtyNodes.reserve(100);

	return CWTRUE;
}

CWBOOL cwSpriteManager::buildRenderObjects()
{
	memset(m_nVertexBuffer, 0, sizeof(m_nVertexBuffer));
	m_pRenderObjects = cwDynamicRenderObject::create(ePrimitiveTypeTriangleList, m_nVertexBuffer, sizeof(cwVertexPosTexColor), CW_DEF_SPRITE_BUFFER_CNT, NULL, 0, "PosTexColor");
	if (!m_pRenderObjects) return CWFALSE;
	CW_SAFE_RETAIN(m_pRenderObjects);

	//cwVertexPosTex spriteBuffer[6];

	//spriteBuffer[0].pos.set(-1.0, -1.0, 0);
	//spriteBuffer[0].tex.set(0, 1.0f);

	//spriteBuffer[1].pos.set(-1.0, 1.0, 0);
	//spriteBuffer[1].tex.set(0, 0);

	//spriteBuffer[2].pos.set(1.0, -1.0, 0);
	//spriteBuffer[2].tex.set(1.0f, 1.0f);

	//spriteBuffer[3].pos.set(-1.0, 1.0, 0);
	//spriteBuffer[3].tex.set(0, 0);

	//spriteBuffer[4].pos.set(1.0, 1.0, 0);
	//spriteBuffer[4].tex.set(1.0f, 0);

	//spriteBuffer[5].pos.set(1.0, -1.0, 0);
	//spriteBuffer[5].tex.set(1.0f, 1.0f);

	//m_pSpriteRenderObject = cwStaticRenderObject::create(ePrimitiveTypeTriangleList, spriteBuffer, sizeof(cwVertexPosTex), 6, NULL, 0, "PosTex");
	//CW_SAFE_RETAIN(m_pSpriteRenderObject);

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

CWBOOL cwSpriteManager::buildBlend()
{
	cwBlendData blendData;

	blendData.bAlphaToCoverage = CWFALSE;
	blendData.bIndependentBlend = CWFALSE;
	blendData.bEnable = CWTRUE;
	blendData.uColorEnable = eColorWriteEnableAll;

	blendData.srcBlend = eBlendFactorOne;
	blendData.dstBlend = eBlendFactorInvSrcAlpha;
	blendData.blendOp = eBlendOpAdd;
	blendData.srcBlendAlpha = eBlendFactorOne;
	blendData.dstBlendAlpha = eBlendFactorZero;
	blendData.blendOpAlpha = eBlendOpAdd;

	m_pAlphaBlend = cwRepertory::getInstance().getDevice()->createBlend(blendData);
	CW_SAFE_RETAIN(m_pAlphaBlend);
	if (m_pAlphaBlend) return CWTRUE;

	return CWFALSE;
}

CWVOID cwSpriteManager::begin()
{
	cwRepertory& repertory = cwRepertory::getInstance();

	m_pOrthoCamera = repertory.getEngine()->getCamera("Ortho");
	m_pCurrCamera = cwRepertory::getInstance().getEngine()->getRenderer()->getCurrCamera();
	repertory.getEngine()->getRenderer()->setCurrCamera(m_pOrthoCamera);
	repertory.getDevice()->disableZBuffer();

	refreshSprite();
}

CWVOID cwSpriteManager::render()
{
	if (m_pRootSprite) {
		renderNode();
	}
}

CWVOID cwSpriteManager::renderNode()
{
	std::stack<cwRenderNode2D*> _stackNodes;
	_stackNodes.push(m_pRootSprite);

	while (!_stackNodes.empty()) {
		cwRenderNode2D* pCurrNode = _stackNodes.top();
		_stackNodes.pop();

		if (!pCurrNode) continue;

		render(pCurrNode);

		cwVector<cwRenderNode*>& nVecChildren = pCurrNode->getChildren();
		std::vector<cwRenderNode2D*> nVecNodes;
		nVecNodes.reserve(nVecChildren.size());

		for (auto pNode : nVecChildren) {
			if (pNode && pNode->getVisible())
				nVecNodes.push_back(static_cast<cwRenderNode2D*>(pNode));
		}

		std::sort(nVecNodes.begin(), nVecNodes.end(), compRenderOrderInvert);

		for (auto pNode2D : nVecNodes)
			_stackNodes.push(pNode2D);
	}
}

CWVOID cwSpriteManager::render(cwRenderNode2D* pNode)
{
	cwRepertory& repertory = cwRepertory::getInstance();

	if (!pNode->getBlend())
		repertory.getDevice()->setBlend(m_pAlphaBlend);
	else
		repertory.getDevice()->setBlend(pNode->getBlend());

	pNode->render(m_pOrthoCamera);
}

CWVOID cwSpriteManager::end()
{
	cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(m_pCurrCamera);
	cwRepertory::getInstance().getDevice()->enableZBuffer();
}

CWVOID cwSpriteManager::refreshSprite()
{
	m_nDirtyNodes.clear();
	if (!m_pRootSprite) return;

	std::vector<cwRenderNode2D*> vecStack;
	vecStack.reserve(10);
	vecStack.push_back(m_pRootSprite);

	while (!vecStack.empty()) {
		cwRenderNode2D* pLast = vecStack.back();
		vecStack.pop_back();
		
		if (pLast->getTransDirty()) {
			addRefreshNode(pLast);
		}
		else {
			cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
			if (!nVecChildren.empty()) {
				for (auto pChildNode : nVecChildren) {
					if (pChildNode && pChildNode->getVisible())
						vecStack.push_back(static_cast<cwRenderNode2D*>(pChildNode));
				}
			}
		}
	}

	for (auto pNode : m_nDirtyNodes) {
		pNode->refreshTransform();
		pNode->refreshBoundingBox();
	}
}

CWVOID cwSpriteManager::addRefreshNode(cwRenderNode2D* pNode)
{
	std::vector<cwRenderNode2D*> vecStack;
	vecStack.reserve(10);
	vecStack.push_back(pNode);

	while (!vecStack.empty()) {
		cwRenderNode2D* pLast = vecStack.back();
		vecStack.pop_back();

		if (pLast->getTransDirty()) {
			pLast->transform();
		}

		m_nDirtyNodes.push_back(pLast);

		cwVector<cwRenderNode*>& nVecChildren = pLast->getChildren();
		if (!nVecChildren.empty()) {
			for (auto pChildNode : nVecChildren) {
				if (pChildNode && pChildNode->getVisible())
					vecStack.push_back(static_cast<cwRenderNode2D*>(pChildNode));
			}
		}
	}
}

CWVOID cwSpriteManager::addNode(cwRenderNode2D* pNode)
{
	if (pNode && m_pRootSprite)
		m_pRootSprite->addChild(pNode);
}

CWVOID cwSpriteManager::removeNode(cwRenderNode2D* pNode)
{
	if (pNode && m_pRootSprite)
		m_pRootSprite->removeChild(pNode);
}

NS_MINIR_END
