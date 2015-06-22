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
#include "cwVertexBuffer.h"
#include "cwCamera.h"
#include "cwRepertory.h"

NS_MINI_BEGIN

cwDynamicRenderObject* cwDynamicRenderObject::create(
ePrimitiveType topology,
CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
CWVOID* pIndexData, CWUINT uIndexCnt,
ceElementDesc eLayout)
{
	cwDynamicRenderObject* pObj = new cwDynamicRenderObject();
	if (pObj && pObj->init( topology, pVertexData, uVertexStride, uVertexCnt, pIndexData, uIndexCnt, eLayout)) {
		pObj->autorelease();
		return pObj;
	}

	CW_SAFE_DELETE(pObj);
	return NULL;
}

cwDynamicRenderObject::cwDynamicRenderObject():
m_pVertexData(NULL)
{

}

cwDynamicRenderObject::~cwDynamicRenderObject()
{
	CW_SAFE_FREE(m_pVertexData);
}

bool cwDynamicRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, ceElementDesc eLayout)
{
	if (!cwRenderObject::init(
		topology,
		pVertexData, uVertexStride, uVertexCnt,
		pIndexData, uIndexCnt, eLayout)) return false;

	cwRenderDevice* pDevice = cwRepertory::getInstance().getDevice();
	m_pVertexBuffer = pDevice->createVertexBuffer(NULL, uVertexStride, uVertexCnt, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	CW_SAFE_RETAIN(m_pVertexBuffer);
	if (!m_pVertexBuffer) return false;

	m_pVertexData = (CWVOID*)malloc(uVertexStride*uVertexCnt);
	if (!m_pVertexData) return false;

	memcpy(m_pVertexData, pVertexData, uVertexStride*uVertexCnt);

	return true;
}

void cwDynamicRenderObject::preRender()
{
	updateVertexBuffer();
}

void cwDynamicRenderObject::updateVertexBuffer()
{
	CW_RES_LOCK_DATA lockData = cwRepertory::getInstance().getDevice()->lockBuffer(m_pVertexBuffer);
	memcpy(lockData.pData, m_pVertexData, m_pVertexBuffer->getSize());
	cwRepertory::getInstance().getDevice()->unlockBuffer(m_pVertexBuffer);
}

NS_MINI_END