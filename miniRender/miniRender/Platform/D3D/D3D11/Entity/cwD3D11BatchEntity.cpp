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

#ifdef _CW_D3D11_

#include "cwD3D11BatchEntity.h"
#include "RenderObject/cwRenderObject.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Repertory/cwRepertory.h"
#include "Layouts/cwLayoutsManager.h"
//#include "Render/cwRenderBatch.h"
#include "Effect/cwEffect.h"

NS_MINIR_BEGIN

cwD3D11BatchEntity* cwD3D11BatchEntity::create()
{
	cwD3D11BatchEntity* pBatchEntity = new cwD3D11BatchEntity();
	if (pBatchEntity && pBatchEntity->init()) {
		pBatchEntity->autorelease();
		return pBatchEntity;
	}

	CW_SAFE_DELETE(pBatchEntity);
	return nullptr;
}

cwD3D11BatchEntity::cwD3D11BatchEntity() : 
m_pInstanceRenderObj(nullptr)
{

}

cwD3D11BatchEntity::~cwD3D11BatchEntity()
{
	CW_SAFE_RELEASE_NULL(m_pInstanceRenderObj);
}

CWBOOL cwD3D11BatchEntity::init()
{
	if (!cwBatchEntity::init()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwD3D11BatchEntity::setEntityData(CWVOID* pData, CWUINT uStride, CWUINT uInstanceCount)
{
	if (!pData || uStride == 0 || uInstanceCount == 0) return CWFALSE;
	if (!m_pRenderObj) return CWFALSE;
	CW_SAFE_RELEASE_NULL(m_pInstanceRenderObj);

	CWSTRING strLayout = cwRepertory::getInstance().getLayoutManager()->getLayoutName(m_pRenderObj->getInputLayout());
	m_pInstanceRenderObj = cwDynamicRenderObject::create(m_pRenderObj->getPrimitiveTopology(), pData, uStride, uInstanceCount, NULL, 0, strLayout);
	if (!m_pInstanceRenderObj) return CWFALSE;
	CW_SAFE_RETAIN(m_pInstanceRenderObj);

	m_nVecRenderObject.clear();
	m_nVecRenderObject.push_back(m_pRenderObj);
	m_nVecRenderObject.push_back(m_pInstanceRenderObj);

	m_uInstanceMaxCount = uInstanceCount;
	m_uInstanceCount = uInstanceCount;

	return CWTRUE;
}

CWVOID cwD3D11BatchEntity::refreshEntityData(CWVOID* pData, CWUINT uInstanceCount)
{
	if (pData && m_pInstanceRenderObj) {
		m_pInstanceRenderObj->updateVertexData(pData);
		m_uInstanceCount = uInstanceCount<=m_uInstanceMaxCount?uInstanceCount:m_uInstanceMaxCount;
	}
}

//CWVOID cwD3D11BatchEntity::render(cwRenderBatch* pRenderBatch)
//{
//	if (pRenderBatch && pRenderBatch->m_pEffect) {
//		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
//		pDevice->draw(pRenderBatch->m_pEffect->getShader(), pRenderBatch->m_pEffect->getTech(), m_nVecRenderObject, m_uInstanceCount);
//	}
//
//	cwRenderNode::render();
//}

CWVOID cwD3D11BatchEntity::render(cwEffect* pEffect)
{
	if (pEffect) {
		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
		//pDevice->draw(pEffect->getShader(), pEffect->getTech(), m_nVecRenderObject, m_uInstanceCount);
		pDevice->draw(pEffect, m_nVecRenderObject, m_uInstanceCount);
	}

	cwRenderNode::render();
}

NS_MINIR_END

#endif
