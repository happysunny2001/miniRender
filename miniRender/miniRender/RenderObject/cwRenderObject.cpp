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
	CWVOID* pIndexData, CWUINT uIndexCnt, const CWSTRING& strLayout)
{
	m_nTopology = topology;
	if (pIndexData) {
		m_pIndexBuffer = cwRepertory::getInstance().getDevice()->createIndexBuffer(pIndexData, sizeof(CWUINT), uIndexCnt);
		CW_SAFE_RETAIN(m_pIndexBuffer);

		if (!m_pIndexBuffer) return CWFALSE;
	}
	m_pLayout = cwRepertory::getInstance().getLayoutManager()->getLayouts(strLayout);

	if (!m_pLayout) return CWFALSE;

	saveBufferData(pVertexData, uVertexStride, uVertexCnt, pIndexData, uIndexCnt);

	return CWTRUE;
}

CWVOID cwRenderObject::updateVertexData(CWVOID* pData)
{
	if (!pData) return;
	memcpy(m_pVertexData, pData, m_uStride*m_uVertexCnt);
}

CWVOID* cwRenderObject::getVertexData(CWUINT i)
{
	if (i >= m_uVertexCnt) return nullptr;

	return (CWBYTE*)m_pVertexData + i*m_uStride;
}

CWVOID cwRenderObject::saveBufferData(
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt, 
	CWVOID* pIndexData, CWUINT uIndexCnt)
{
	CW_SAFE_FREE(m_pVertexData);
	CW_SAFE_FREE(m_pIndexData);

	if (pVertexData) {
		m_pVertexData = (CWVOID*)malloc(uVertexStride*uVertexCnt);
		if (m_pVertexData) {
			memcpy(m_pVertexData, pVertexData, uVertexStride*uVertexCnt);

			m_uStride = uVertexStride;
			m_uVertexCnt = uVertexCnt;
		}
	}

	if (pIndexData) {
		m_pIndexData = (CWUINT*)malloc(sizeof(CWUINT)*uIndexCnt);
		if (m_pIndexData) {
			memcpy(m_pIndexData, pIndexData, sizeof(CWUINT)*uIndexCnt);
			m_uIndexCnt = uIndexCnt;
		}
	}
}

NS_MINIR_END
