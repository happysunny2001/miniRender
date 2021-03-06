﻿/*
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
#include "Buffer/cwBuffer.h"
#include "Base/cwMacros.h"
#include "Device/cwDevice.h"
#include "Layouts/cwLayoutsManager.h"
#include "Repertory/cwRepertory.h"

NS_MINIR_BEGIN

cwStaticRenderObject* cwStaticRenderObject::create(
		ePrimitiveType topology,
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt, const CWSTRING& strLayout, CWUINT uPositionOffset)
{
	cwStaticRenderObject* pObj = new cwStaticRenderObject();
	if (pObj && pObj->init(topology, pVertexData, uVertexStride, uVertexCnt, pIndexData, uIndexCnt, strLayout, uPositionOffset)) {
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

CWBOOL cwStaticRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, 
	const CWSTRING& strLayout, CWUINT uPositionOffset)
{
	if (!cwRenderObject::init(
		topology, 
		pVertexData, uVertexStride, uVertexCnt, 
		pIndexData, uIndexCnt, strLayout, uPositionOffset)) return CWFALSE;

	m_pVertexBuffer = cwRepertory::getInstance().getDevice()->createVertexBuffer(pVertexData, uVertexStride, uVertexCnt);
	CW_SAFE_RETAIN(m_pVertexBuffer);
	m_pVertexBuffer->setOffset(uPositionOffset);

	return m_pVertexBuffer ? CWTRUE : CWFALSE;
}

NS_MINIR_END
