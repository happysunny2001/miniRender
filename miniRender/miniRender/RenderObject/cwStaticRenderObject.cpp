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

#include "cwStaticRenderObject.h"
#include "cwVertexBuffer.h"
#include "Base/cwMacros.h"
#include "Device/cwDevice.h"
#include "cwEffectManager.h"
#include "Layouts/cwLayoutsManager.h"
#include "cwRepertory.h"

NS_MINI_BEGIN

cwStaticRenderObject* cwStaticRenderObject::create(
		ePrimitiveType topology,
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt, ceElementDesc eLayout)
{
	cwStaticRenderObject* pObj = new cwStaticRenderObject();
	if (pObj && pObj->init(topology, pVertexData, uVertexStride, uVertexCnt, pIndexData, uIndexCnt, eLayout)) {
		pObj->autorelease();
		return pObj;
	}

	CW_SAFE_DELETE(pObj);
	return NULL;
}

cwStaticRenderObject::cwStaticRenderObject()
{

}

cwStaticRenderObject::~cwStaticRenderObject()
{

}

bool cwStaticRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, ceElementDesc eLayout)
{
	if (!cwRenderObject::init(
		topology, 
		pVertexData, uVertexStride, uVertexCnt, 
		pIndexData, uIndexCnt, eLayout)) return false;
	m_pVertexBuffer = cwRepertory::getInstance().getDevice()->createVertexBuffer(pVertexData, uVertexStride, uVertexCnt);
	CW_SAFE_RETAIN(m_pVertexBuffer);

	return m_pVertexBuffer ? true : false;
}

NS_MINI_END