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

#ifndef _CW_RENDEROBJECT_H_
#define _CW_RENDEROBJECT_H_

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class cwBuffer;
class cwLayouts;
class cwCamera;

class CW_DLL cwRenderObject : public cwRef
{
public:
	cwRenderObject();
	virtual ~cwRenderObject();

	virtual CWBOOL init(
		ePrimitiveType topology,
		CWVOID* pVertexData, CWUINT uVertexStride, CWUINT uVertexCnt, 
		CWVOID* pIndexData, CWUINT uIndexCnt, ceElementDesc eLayout) = 0;

	virtual CWVOID preRender() {}

	inline cwLayouts* getInputLayout() { return m_pLayout; }
	inline ePrimitiveType getPrimitiveTopology() { return m_nTopology; }

	inline cwBuffer* getVertexBuffer() { return m_pVertexBuffer; }
	inline cwBuffer* getIndexBuffer() { return m_pIndexBuffer; }

protected:
	cwBuffer* m_pVertexBuffer;
	cwBuffer* m_pIndexBuffer;
	cwLayouts* m_pLayout;
	ePrimitiveType m_nTopology;

};

NS_MINIR_END

#endif
