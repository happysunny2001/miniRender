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
#include "Base/cwStruct.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Entity/cwScene.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Render/cwRenderer.h"

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
m_pRenderObjects(nullptr)
{

}

cwSpriteManager::~cwSpriteManager()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObjects);
}

CWBOOL cwSpriteManager::init()
{
	if (!buildRenderObjects()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwSpriteManager::buildRenderObjects()
{
	m_pRenderObjects = cwDynamicRenderObject::create(ePrimitiveTypeTriangleList, nullptr, sizeof(cwVertexPosTexColor), 1000, NULL, 0, "PosTexColor");
	if (!m_pRenderObjects) return CWFALSE;
	CW_SAFE_RETAIN(m_pRenderObjects);

	return CWTRUE;
}

CWVOID cwSpriteManager::begin()
{
	m_nVecSprites.clear();

	cwScene* pScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	if (pScene) {
		pScene->getRenderNode(eSceneObjectSprite, m_nVecSprites);
	}

	cwCamera* pOrthoCamera = cwRepertory::getInstance().getEngine()->getCamera("Ortho");
	m_pCurrCamera = cwRepertory::getInstance().getEngine()->getRenderer()->getCurrCamera();
	cwRepertory::getInstance().getEngine()->getRenderer()->setCurrCamera(pOrthoCamera);
}

CWVOID cwSpriteManager::render()
{

}

CWVOID cwSpriteManager::end()
{
	m_nVecSprites.clear();
}

NS_MINIR_END
