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

#include "cwRenderObject.h"
#include "Buffer/cwBuffer.h"
#include "Layouts/cwLayouts.h"
#include "Device/cwDevice.h"
#include "Layouts/cwLayoutsManager.h"
#include "Repertory/cwRepertory.h"

NS_MINIR_BEGIN

cwRenderObject::cwRenderObject():
m_pVertexBuffer(nullptr),
m_pIndexBuffer(nullptr),
m_pLayout(nullptr),
m_pVertexData(nullptr),
m_pIndexData(nullptr),
m_uStride(0),
m_uVertexCnt(0),
m_uIndexCnt(0)
{
}

cwRenderObject::~cwRenderObject()
{
	CW_SAFE_RELEASE_NULL(m_pVertexBuffer);
	CW_SAFE_RELEASE_NULL(m_pIndexBuffer);

	CW_SAFE_FREE(m_pVertexData);
	CW_SAFE_FREE(m_pIndexData);

	m_uStride = 0;
	m_uVertexCnt = 0;
	m_uIndexCnt = 0;
}

CWBOOL cwRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, 
	const CWSTRING& strLayout, CWUINT uPositionOffset)
{
	m_nTopology = topology;
	m_pLayout = cwRepertory::getInstance().getLayoutManager()->getLayouts(strLayout);
	if (!buildIndexBuffer(pIndexData, uIndexCnt)) return CWFALSE;

	if (!m_pLayout) return CWFALSE;

	m_nAabb.zero();
	saveBufferData(pVertexData, uVertexStride, uVertexCnt, uPositionOffset, pIndexData, uIndexCnt);

	return CWTRUE;
}

CWBOOL cwRenderObject::buildIndexBuffer(CWVOID* pIndexData, CWUINT uIndexCnt)
{
	if (pIndexData) {
		m_pIndexBuffer = cwRepertory::getInstance().getDevice()->createIndexBuffer(pIndexData, sizeof(CWUINT), uIndexCnt);
		CW_SAFE_RETAIN(m_pIndexBuffer);

		if (!m_pIndexBuffer) return CWFALSE;
		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwRenderObject::rebuild(
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt,
	CWUINT uPositionOffset)
{
	return CWFALSE;
}

CWVOID cwRenderObject::updateVertexData(CWVOID* pData)
{
	updateVertexData(pData, m_uStride*m_uVertexCnt);
}

CWVOID cwRenderObject::updateVertexData(CWVOID* pData, CWUINT uSize)
{
	if (pData) {
		memcpy(m_pVertexData, pData, uSize);
	}
}

CWVOID* cwRenderObject::getVertexData(CWUINT i)
{
	if (i < m_uVertexCnt) {
		return (CWBYTE*)m_pVertexData + i*m_uStride;
	}

	return nullptr;
}

CWVOID cwRenderObject::updateIndexData(CWVOID* pData, CWUINT uSize)
{
	if (pData) {
		memcpy(m_pIndexBuffer, pData, uSize);
	}
}

CWVOID cwRenderObject::saveBufferData(
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt, CWUINT uPositionOffset,
	CWVOID* pIndexData, CWUINT uIndexCnt)
{
	CW_SAFE_FREE(m_pVertexData);
	CW_SAFE_FREE(m_pIndexData);

	m_uStride = uVertexStride;
	m_uVertexCnt = uVertexCnt;
	m_pVertexData = (CWVOID*)malloc(uVertexStride*uVertexCnt);

	if (m_pVertexData) {
		if (pVertexData)
			memcpy(m_pVertexData, pVertexData, uVertexStride*uVertexCnt);
		else
			memset(m_pVertexData, 0, uVertexStride*uVertexCnt);
	}

	m_uIndexCnt = uIndexCnt;

	if (pIndexData) {
		m_pIndexData = (CWUINT*)malloc(sizeof(CWUINT)*uIndexCnt);
		if (m_pIndexData)
			memcpy(m_pIndexData, pIndexData, sizeof(CWUINT)*uIndexCnt);
	}

	m_uPositionOffset = uPositionOffset;
	calBoundingBox();
}

CWVOID cwRenderObject::calBoundingBox()
{
	if (!m_pVertexData) return;

	m_nAabb.empty();

	CWBYTE* pStart = &((CWBYTE*)m_pVertexData)[m_uPositionOffset];
	for (CWUINT i = 0; i < m_uVertexCnt; ++i) {
		cwVector3D* pos = (cwVector3D*)pStart;

		m_nAabb.add(*pos);

		pStart += m_uStride;
	}
}

NS_MINIR_END
