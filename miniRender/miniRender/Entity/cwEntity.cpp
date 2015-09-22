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

#include "cwEntity.h"
#include "RenderObject/cwRenderObject.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Material/cwMaterial.h"
#include "Light/cwLight.h"
#include "Shader/cwShader.h"
#include "Engine/cwEngine.h"
#include "Blend/cwBlend.h"
#include "Stencil/cwStencil.h"
#include "Render/cwRenderer.h"
#include "Render/cwRenderBatch.h"
#include "effect/cwEffect.h"
#include "Engine/cwEngine.h"

NS_MINIR_BEGIN

cwEntity* cwEntity::create()
{
	cwEntity* pEntity = new cwEntity();
	if (pEntity && pEntity->init()) {
		pEntity->autorelease();
		return pEntity;
	}

	CW_SAFE_DELETE(pEntity);
	return nullptr;
}

cwEntity::cwEntity():
m_pRenderObj(nullptr),
m_pMaterial(nullptr),
m_pBlend(nullptr),
m_pStencil(nullptr)
{
	m_eType = eSceneObjectEntity;
}

cwEntity::~cwEntity()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObj);
	CW_SAFE_RELEASE_NULL(m_pMaterial);
	CW_SAFE_RELEASE_NULL(m_pBlend);
	CW_SAFE_RELEASE_NULL(m_pStencil);
}

CWBOOL cwEntity::init()
{
	if (!cwRenderNode::init()) return CWFALSE;

	return CWTRUE;
}

CWVOID cwEntity::transform()
{
	if (m_bTransDirty) {
		cwRenderNode::transform();
		m_nAabb.update(m_pRenderObj->getBoundingBox(), m_nTrans);
	}
}

CWVOID cwEntity::setRenderObject(cwRenderObject* pRenderObj)
{
	if (pRenderObj == m_pRenderObj) return;

	CW_SAFE_RETAIN(pRenderObj);
	CW_SAFE_RELEASE_NULL(m_pRenderObj);
	m_pRenderObj = pRenderObj;
}

CWVOID cwEntity::setMaterial(cwMaterial* pMaterial)
{
	if (pMaterial == m_pMaterial) return;

	CW_SAFE_RETAIN(pMaterial);
	CW_SAFE_RELEASE_NULL(m_pMaterial);
	m_pMaterial = pMaterial;
}

CWVOID cwEntity::setBlend(cwBlend* pBlend)
{
	if (m_pBlend == pBlend) return;

	CW_SAFE_RETAIN(pBlend);
	CW_SAFE_RELEASE_NULL(m_pBlend);
	m_pBlend = pBlend;
}

CWVOID cwEntity::setStencil(cwStencil* pStencil)
{
	if (m_pStencil == pStencil) return;

	CW_SAFE_RETAIN(pStencil);
	CW_SAFE_RELEASE_NULL(m_pStencil);
	m_pStencil = pStencil;
}

CWVOID cwEntity::render(cwRenderBatch* pRenderBatch)
{
	if (pRenderBatch && pRenderBatch->m_pEffect) {
		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
		pDevice->draw(pRenderBatch->m_pEffect->getShader(), pRenderBatch->m_pEffect->getTech(), m_pRenderObj);
	}

	//cwRepertory::getInstance().getEngine()->getRenderer()->renderPrimitive(m_nAabb);
	cwRenderNode::render();
}

NS_MINIR_END