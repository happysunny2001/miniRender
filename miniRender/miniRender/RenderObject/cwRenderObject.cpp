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

#include "cwRenderObject.h"
#include "cwVertexBuffer.h"
#include "cwIndexBuffer.h"
#include "Layouts/cwLayouts.h"
#include "Device/cwDevice.h"
#include "cwEffectManager.h"
#include "Layouts/cwLayoutsManager.h"
#include "cwCamera.h"
#include "cwShaderConstant.h"
#include "cwRepertory.h"

NS_MINI_BEGIN

cwRenderObject::cwRenderObject():
m_pVertexBuffer(nullptr),
m_pIndexBuffer(nullptr),
m_pLayout(nullptr)
{
}

cwRenderObject::~cwRenderObject()
{
	CW_SAFE_RELEASE_NULL(m_pVertexBuffer);
	CW_SAFE_RELEASE_NULL(m_pIndexBuffer);
}

bool cwRenderObject::init(
	ePrimitiveType topology,
	CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
	CWVOID* pIndexData, CWUINT uIndexCnt, ceElementDesc eLayout)
{
	m_nTopology = topology;
	m_pIndexBuffer = cwRepertory::getInstance().getDevice()->createIndexBuffer(pIndexData, sizeof(CWUINT), uIndexCnt);
	CW_SAFE_RETAIN(m_pIndexBuffer);
	m_pLayout = cwRepertory::getInstance().getLayoutManager()->getLayouts(eLayout);

	if (!m_pIndexBuffer || !m_pLayout) return false;

	return true;
}

NS_MINI_END