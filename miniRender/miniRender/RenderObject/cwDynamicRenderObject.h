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

#ifndef _CW_DYNAMICRENDEROBJECT_H_
#define _CW_DYNAMICRENDEROBJECT_H_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "cwRenderObject.h"

NS_MINIR_BEGIN

class cwCamera;

class CW_DLL cwDynamicRenderObject : public cwRenderObject
{
public:
	static cwDynamicRenderObject* create(
		ePrimitiveType topology,
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt, const CWSTRING& strLayout, CWUINT uPositionOffset = 0);

	cwDynamicRenderObject();
	virtual ~cwDynamicRenderObject();

	virtual CWBOOL init(
		ePrimitiveType topology,
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt, const CWSTRING& strLayout, CWUINT uPositionOffset = 0) override;

	virtual CWBOOL rebuild(
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt,
		CWVOID* pIndexData, CWUINT uIndexCnt,
		CWUINT uPositionOffset = 0) override;

	virtual CWVOID preRender() override;
	virtual CWVOID updateVertexData(CWVOID* pData, CWUINT iCnt) override;
	virtual CWVOID updateIndexData(CWVOID* pData, CWUINT iCnt) override;

protected:
	virtual CWBOOL buildIndexBuffer(CWVOID* pIndexData, CWUINT uIndexCnt) override;

protected:
	CWBOOL m_bVertexRefresh;
	CWUINT m_uVertexRefreshSize;

	CWBOOL m_bIndexRefresh;
	CWUINT m_uIndexRefreshSize;

};

NS_MINIR_END

#endif