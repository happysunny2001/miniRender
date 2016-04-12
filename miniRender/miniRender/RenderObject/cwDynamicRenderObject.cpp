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

#include "cwDynamicRenderObject.h"
#include "Device/cwDevice.h"
#include "Camera/cwCamera.h"
#include "Buffer/cwBuffer.h"
#include "Repertory/cwRepertory.h"

NS_MINIR_BEGIN

cwDynamicRenderObject* cwDynamicRenderObject::create(
ePrimitiveType topology,
CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
CWVOID* pIndexData, CWUINT uIndexCnt,
const CWSTRING& strLayout, CWUINT uPositionOffset)
{
	cwDynamicRenderObject* pObj = new cwDynamicRenderObject();
	if (pObj && pObj->init(topology, pVertexData, uVertexStride, uVertexCnt, pIndexData, uIndexCnt, strLayout, uPositionOffset)) {
		pObj->autorelease();
		return pObj;
	}

	CW_SAFE_DELETE(pObj);
	return NULL;
}

cwDynamicRenderObject::cwDynamicRenderObject() :
m_bVertexRefresh(CWFALSE),
m_uVertexRefreshSize(0),
m_bIndexRefresh(CWFALSE),
m_uIndexRefreshSize(0)
{

}

cwDynamicRenderObject::~cwDynamicRenderObject()
{

}

CWBOOL cwDynamicRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, const CWSTRING& strLayout, CWUINT uPositionOffset)
{
	if (!cwRenderObject::init(
		topology,
		pVertexData, uVertexStride, uVertexCnt,
		pIndexData, uIndexCnt, strLayout, uPositionOffset)) return CWFALSE;

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	m_pVertexBuffer = pDevice->createVertexBuffer(pVertexData, uVertexStride, uVertexCnt, eBufferUsageDynamic, eAccessFlagWrite);
	CW_SAFE_RETAIN(m_pVertexBuffer);
	if (!m_pVertexBuffer) return CWFALSE;
	m_pVertexBuffer->setOffset(uPositionOffset);

	return CWTRUE;
}

CWBOOL cwDynamicRenderObject::rebuild(
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt,
	CWUINT uPositionOffset)
{
	CWBOOL bVert = CWTRUE;
	CWBOOL bIndex = CWTRUE;

	if (pVertexData) {
		bVert = m_pVertexBuffer->rebuild(
			pVertexData, uVertexStride*uVertexCnt,
			eBufferUsageDynamic, eBufferBindVertex, eAccessFlagWrite, 0, uVertexStride, uPositionOffset);
	}

	if (pIndexData) {
		bIndex = m_pIndexBuffer->rebuild(
			pIndexData, sizeof(CWUINT)*uIndexCnt,
			eBufferUsageDynamic, eBufferBindIndex, eAccessFlagWrite, 0, sizeof(CWUINT), 0);
	}

	saveBufferData(pVertexData, uVertexStride, uVertexCnt, uPositionOffset, pIndexData, uIndexCnt);

	return bVert&&bIndex;
}

CWBOOL cwDynamicRenderObject::buildIndexBuffer(CWVOID* pIndexData, CWUINT uIndexCnt)
{
	if (pIndexData) {
		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
		m_pIndexBuffer = pDevice->createIndexBuffer(pIndexData, sizeof(CWUINT), uIndexCnt, eBufferUsageDynamic, eAccessFlagWrite);
		CW_SAFE_RETAIN(m_pIndexBuffer);

		if (!m_pIndexBuffer) return CWFALSE;
		return CWTRUE;
	}

	return CWTRUE;
}

CWVOID cwDynamicRenderObject::preRender()
{
	if (m_bVertexRefresh) {
		m_pVertexBuffer->refresh(m_pVertexData, m_uVertexRefreshSize);
		m_bVertexRefresh = CWFALSE;
		m_uVertexRefreshSize = 0;
	}

	if (m_bIndexRefresh) {
		m_pIndexBuffer->refresh(m_pIndexData, m_uIndexRefreshSize);
		m_bIndexRefresh = CWFALSE;
		m_uIndexRefreshSize = 0;
	}
}

CWVOID cwDynamicRenderObject::updateVertexData(CWVOID* pData, CWUINT iCnt)
{
	cwRenderObject::updateVertexData(pData, iCnt);
	m_bVertexRefresh = CWTRUE;
	m_uVertexRefreshSize = iCnt*m_uStride;
}

CWVOID cwDynamicRenderObject::updateIndexData(CWVOID* pData, CWUINT iCnt)
{
	cwRenderObject::updateIndexData(pData, iCnt);
	m_bIndexRefresh = CWTRUE;
	m_uIndexRefreshSize = iCnt*sizeof(CWUINT);
}

NS_MINIR_END
