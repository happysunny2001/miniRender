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
const CWSTRING& strLayout)
{
	cwDynamicRenderObject* pObj = new cwDynamicRenderObject();
	if (pObj && pObj->init( topology, pVertexData, uVertexStride, uVertexCnt, pIndexData, uIndexCnt, strLayout)) {
		pObj->autorelease();
		return pObj;
	}

	CW_SAFE_DELETE(pObj);
	return NULL;
}

cwDynamicRenderObject::cwDynamicRenderObject() :
m_bVertexRefresh(CWTRUE)
{

}

cwDynamicRenderObject::~cwDynamicRenderObject()
{

}

CWBOOL cwDynamicRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, const CWSTRING& strLayout)
{
	if (!cwRenderObject::init(
		topology,
		pVertexData, uVertexStride, uVertexCnt,
		pIndexData, uIndexCnt, strLayout)) return CWFALSE;

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	m_pVertexBuffer = pDevice->createVertexBuffer(NULL, uVertexStride, uVertexCnt, eBufferUsageDynamic, eAccessFlagWrite);
	CW_SAFE_RETAIN(m_pVertexBuffer);
	if (!m_pVertexBuffer) return CWFALSE;

	return CWTRUE;
}

void cwDynamicRenderObject::preRender()
{
	if (m_bVertexRefresh) {
		updateVertexBuffer();
		m_bVertexRefresh = CWFALSE;
	}
}

CWVOID cwDynamicRenderObject::updateVertexData(CWVOID* pData)
{
	cwRenderObject::updateVertexData(pData);
	m_bVertexRefresh = CWTRUE;
}

void cwDynamicRenderObject::updateVertexBuffer()
{
	m_pVertexBuffer->refresh(m_pVertexData);
}

NS_MINIR_END
